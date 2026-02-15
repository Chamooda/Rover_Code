# Rover Control System

A web-based remote control system for managing a WiFi-enabled rover with DC motor control using an ESP32 microcontroller and L298N motor driver module.

## Project Overview

This project provides a complete solution for controlling a rover robot through a web interface. The system consists of:

- **Web Server**: Node.js server with WebSocket support for real-time communication
- **Web Interface**: HTML-based control dashboard with motor speed and direction controls
- **Microcontroller Code**: Arduino sketches for both standard Arduino and ESP32 platforms
- **Motor Driver**: L298N dual H-bridge motor control library and implementation
- **Autonomy Module**: Framework for future autonomous operation (currently under development)

## Hardware Requirements

- **Microcontroller**: ESP32 or Arduino board
- **Motor Driver**: L298N Dual H-Bridge Motor Driver Module
- **Motors**: 2x DC Motors (for rover movement)
- **Power Supply**: Suitable power source for motors and microcontroller
- **WiFi**: WiFi connectivity for remote control via web interface
- **Connectivity**: Serial/USB connection for programming

## Project Structure

```
Rover_Code/
├── server.js                          # Node.js WebSocket server
├── package.json                       # Project dependencies
├── Rover.ino                          # Main Arduino sketch for rover control
├── index.html                         # Web control interface (main)
├── controls.html                      # Additional control interface
├── Koshish.html                       # Alternative control interface
├── oogabooga.html                     # Additional UI variant
├── ESP32/                             # ESP32-specific implementation
│   ├── ESP32.ino                      # ESP32 main sketch
│   ├── ESP32_L298N_DC_motor_wifi_page.h  # WiFi page header
│   ├── Robojax_L298N_DC_motor.h       # Motor control library header
│   ├── Robojax_L298N_DC_motor.cpp     # Motor control library implementation
│   ├── library.properties             # Arduino library metadata
│   ├── keywords.txt                   # Syntax highlighting keywords
│   ├── README.md                      # ESP32-specific documentation
│   └── LICENSE                        # Library license
├── Motors_Control/                    # Motor control module
│   ├── README.md                      # Motor control documentation
│   └── ESP/
│       └── ESP.ino                    # ESP-specific motor control sketch
└── Autonomy(not_implemented_yet)/     # Autonomous operation framework
    └── Embedded_tracking.py           # Autonomous tracking module (placeholder)
```

## Features

### Web Interface
- Real-time motor control via WebSocket communication
- Adjustable motor speed with slider controls
- Direction control (forward, backward, left, right, stop)
- Multiple control page variants for different use cases
- Responsive HTML5 interface

### Motor Control
- Support for dual DC motors using L298N driver
- Variable speed control (PWM)
- Bidirectional motor control
- Robojax library integration for simplified control

### Connectivity
- WebSocket-based real-time communication
- ESP32 WiFi support
- Serial communication capabilities
- Local network control via web browser

## Installation

### Prerequisites
- Node.js and npm
- Arduino IDE or PlatformIO
- ESP32 board support (if using ESP32)
- USB cable for microcontroller programming

### Server Setup
```bash
# Install dependencies
npm install

# Start the WebSocket server
node server.js
```

The server will listen on:
- HTTP: port 8082
- WebSocket: port 8082

### Microcontroller Setup

#### For Arduino:
1. Open `Rover.ino` in Arduino IDE
2. Configure motor pins as needed
3. Upload to your Arduino board

#### For ESP32:
1. Open `ESP32/ESP32.ino` in Arduino IDE
2. Install ESP32 board support
3. Configure WiFi SSID and password
4. Upload to your ESP32 board

## Usage

### Accessing the Web Interface
1. Ensure the Node.js server is running: `node server.js`
2. Open a web browser and navigate to `http://localhost:8082`
3. Select the desired control interface (index.html, controls.html, etc.)
4. Use the sliders and buttons to control motor speed and direction

### Motor Control
- **Speed**: Adjust the slider to control PWM speed (0-255)
- **Direction**: Select forward, backward, left turn, or right turn
- **Stop**: Center the slider or press the stop button

## Configuration

### Motor Pins (Rover.ino)
```cpp
// Motor A
int enA = 9;   // Enable (PWM)
int in1 = 8;   // Direction
int in2 = 7;   // Direction

// Motor B
int enB = 3;   // Enable (PWM)
int in3 = 5;   // Direction
int in4 = 4;   // Direction
```

### Server Port Configuration
Edit `server.js` to change ports:
```javascript
const port = 8083;  // WebSocket server port
```

## Future Development

### Autonomy Module
The `Autonomy(not_implemented_yet)` folder contains the framework for autonomous operation:
- `Embedded_tracking.py` - Computer vision-based object tracking (placeholder)
- Future: Integration with OpenCV or similar libraries for autonomous navigation

## Dependencies

### Node.js
- **websocket**: ^1.0.34 - WebSocket server implementation

### Arduino
- Robojax L298N DC Motor Library (included in ESP32 folder)
- ESP32 Board Support Package

## Troubleshooting

### WebSocket Connection Issues
- Ensure the Node.js server is running
- Check firewall settings
- Verify the correct IP address and port

### Motor Not Responding
- Check motor power supply connections
- Verify L298N driver connections to microcontroller
- Test motor with direct power connection
- Check PWM pin configuration

### ESP32 WiFi Connection
- Verify SSID and password in the sketch
- Check WiFi signal strength
- Ensure ESP32 has power supply capability for WiFi

## Hardware Pinout Reference

### L298N Motor Driver Connections
```
L298N    →    ESP32/Arduino
OUT1     →    Motor A (+)
OUT2     →    Motor A (-)
OUT3     →    Motor B (+)
OUT4     →    Motor B (-)
IN1      →    GPIO Pin 5/in1
IN2      →    GPIO Pin 4/in2
IN3      →    GPIO Pin 3/in3
IN4      →    GPIO Pin 2/in4
ENA      →    PWM Pin 9/enA
ENB      →    PWM Pin 3/enB
GND      →    GND
```

## License

This project includes components licensed under the GNU General Public License v3.0 (see ESP32/LICENSE for details).

## Contributing

Feel free to fork this project and submit pull requests for improvements, bug fixes, or new features.

## Support

For issues, questions, or suggestions, please open an issue on the GitHub repository.

---

**Note**: The Autonomy module is currently under development. Future versions will include autonomous navigation capabilities.
