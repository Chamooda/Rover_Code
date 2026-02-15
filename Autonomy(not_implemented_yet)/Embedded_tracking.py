import cv2
import numpy as np
import math
import matplotlib.pyplot as plt

# === Crop Settings ===
CROP_HEIGHT = 360  # End of cropped region
CROP_Start = 50    # Start of cropped region

# === ORB & Matcher ===
orb = cv2.ORB_create()
bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

# === Tracking Vars ===
prev_gray = None
prev_kp = None
prev_des = None
x_pos, y_pos, angle = 0, 0, 0
trajectory = [(x_pos, y_pos)]  # List of (x, y) positions

# === Processing Functions ===
def stabilize_frames(prev_gray, curr_gray):
    prev_pts = cv2.goodFeaturesToTrack(prev_gray, maxCorners=200, qualityLevel=0.01, minDistance=30)
    if prev_pts is None:
        return np.eye(2, 3, dtype=np.float32)
    curr_pts, status, _ = cv2.calcOpticalFlowPyrLK(prev_gray, curr_gray, prev_pts, None)
    good_prev = prev_pts[status == 1]
    good_curr = curr_pts[status == 1]
    transform = cv2.estimateAffinePartial2D(good_prev, good_curr)[0]
    return transform if transform is not None else np.eye(2, 3, dtype=np.float32)

def apply_stabilization(frame, transform):
    h, w = frame.shape[:2]
    return cv2.warpAffine(frame, transform, (w, h), flags=cv2.INTER_LINEAR, borderMode=cv2.BORDER_REFLECT)

# === Main Processing + ORB Tracking ===
def process_and_track(input_path):
    global prev_gray, prev_kp, prev_des, x_pos, y_pos, angle, trajectory

    cap = cv2.VideoCapture(input_path)
    if not cap.isOpened():
        print("Error: Could not open video.")
        return

    ret, prev_frame = cap.read()
    if not ret:
        print("Error: Empty video.")
        return

    prev_frame = prev_frame[CROP_Start:CROP_HEIGHT, :, :]
    prev_gray = cv2.cvtColor(prev_frame, cv2.COLOR_BGR2GRAY)

    print("Press 'q' to quit.")
    while True:
        ret, frame = cap.read()
        if not ret:
            break

        # === Crop and Preprocess ===
        #frame = frame[CROP_Start:CROP_HEIGHT, :, :]
        curr_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # transform = stabilize_frames(prev_gray, curr_gray)
        # stabilized = apply_stabilization(frame, transform)

        # === ORB Tracking on Processed Frame ===
        gray = cv2.cvtColor(frame , cv2.COLOR_BGR2GRAY)
        kp, des = orb.detectAndCompute(gray, None)

        if prev_kp is not None and prev_des is not None and des is not None:
            matches = bf.match(prev_des, des)
            matches = sorted(matches, key=lambda x: x.distance)

            if len(matches) > 10:
                src_pts = np.float32([prev_kp[m.queryIdx].pt for m in matches]).reshape(-1, 1, 2)
                dst_pts = np.float32([kp[m.trainIdx].pt for m in matches]).reshape(-1, 1, 2)

                matrix, _ = cv2.estimateAffinePartial2D(src_pts, dst_pts)
                if matrix is not None:
                    dx, dy = matrix[0, 2], matrix[1, 2]
                    dtheta = math.degrees(math.atan2(matrix[1, 0], matrix[0, 0]))

                    rotation_factor = max(0.2, 1 - abs(dtheta) / 90)
                    dx *= rotation_factor
                    dy *= rotation_factor

                    dx_rot = dx * math.cos(math.radians(angle)) - dy * math.sin(math.radians(angle))
                    dy_rot = dx * math.sin(math.radians(angle)) + dy * math.cos(math.radians(angle))

                    x_pos += dx_rot * 0.5
                    y_pos += dy_rot * 0.5
                    angle += dtheta

                    trajectory.append((x_pos, y_pos))

        prev_kp, prev_des = kp, des
        prev_gray = gray.copy()

        # === Overlay Visualization ===
        frame_overlay = cv2.drawKeypoints(frame, kp, None, color=(0, 255, 0))
        text = f"Position: ({x_pos:.2f}, {y_pos:.2f}), Angle: {angle:.2f}°"
        cv2.putText(frame_overlay, text, (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2)

        center_x, center_y = int(frame.shape[1] / 2), int(frame.shape[0] / 2)
        arrow_length = 50
        end_x = int(center_x + arrow_length * math.cos(math.radians(angle)))
        end_y = int(center_y + arrow_length * math.sin(math.radians(angle)))
        cv2.arrowedLine(frame_overlay, (center_x, center_y), (end_x, end_y), (0, 0, 255), 3)

        cv2.imshow("Stabilized + ORB Tracking", frame_overlay)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

    # === Plot Trajectory ===
    trajectory_np = np.array(trajectory)
    plt.figure(figsize=(8, 8))
    plt.plot(trajectory_np[:, 0], trajectory_np[:, 1], marker="o", linestyle="-", color="b")
    plt.xlabel("X Position")
    plt.ylabel("Y Position")
    plt.title("Movement Trajectory")
    plt.grid()
    plt.show()

# === Run It ===
input_video_path = r"footages\Best_footage.mp4"  # <- update path if needed
process_and_track(input_video_path)
