  #include <WiFi.h>

#include <WebSocketClient.h>

const char* ssid     = "Skyguy";
const char* password = "TheForce";
char path[] = "/";
char host[] = "192.168.215.104:8082";
String IP ="192.168.215.104";
WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

int motor1Pin1=20;
int motor1Pin2=21;
int Enable1Pin=14;

int motor2Pin1=15;
int motor2Pin2=16;
int Enable2Pin=17;

const int freq= 30000;
const int pwmChannel=0;
const int resolution=8;
int Speed=200;


void setup() {
  Serial.begin(115200);
  delay(10);


  //Setting up motor driver and controls
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(Enable1Pin, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(Enable2Pin, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(Enable1Pin, pwmChannel);
  ledcAttachPin(Enable2Pin, pwmChannel);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  String IP=WiFi.localIP().toString().c_str();
  Serial.println(IP);

  delay(5000);
  

  // Connect to the websocket server
  if (client.connect("192.168.215.104", 8082)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    while(1) {
      // Hang on failure
    }
  }

  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    while(1) {
      // Hang on failure
    }  
  }

  if (client.connected())
  {
    webSocketClient.sendData("ESP:"+IP);
  }

}

void Reset_Motors()
{
  digitalWrite(motor1Pin1,LOW);
  digitalWrite(motor1Pin2,LOW);
  digitalWrite(motor2Pin1,LOW);
  digitalWrite(motor2Pin2,LOW);  

}

void Forward()
{
  Reset_Motors();
  ledcWrite(pwmChannel, Speed);
  digitalWrite(motor1Pin1,HIGH);
  digitalWrite(motor1Pin2,LOW);
  digitalWrite(motor2Pin1,HIGH);
  digitalWrite(motor2Pin2,LOW);

}

void Left()
{
  Reset_Motors();
  ledcWrite(pwmChannel,Speed);
  digitalWrite(motor1Pin1,HIGH);
  digitalWrite(motor1Pin2,LOW);
  digitalWrite(motor2Pin1,LOW);
  digitalWrite(motor2Pin2,HIGH);
}

void Right()
{
  Reset_Motors();
  ledcWrite(pwmChannel,Speed);
  digitalWrite(motor1Pin1,LOW);
  digitalWrite(motor1Pin2,HIGH);
  digitalWrite(motor2Pin1,HIGH);
  digitalWrite(motor2Pin2,LOW);
}

void Backward()
{
  Reset_Motors();
  ledcWrite(pwmChannel, Speed);
  digitalWrite(motor1Pin1,LOW);
  digitalWrite(motor1Pin2,HIGH);
  digitalWrite(motor2Pin1,LOW);
  digitalWrite(motor2Pin2,HIGH);

}

void loop() {
  String data;
  StaticJsonBuffer<300> JSONBuffer;
  String Direction;
  int speed;
  if (client.connected()) {
    
    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
    }



    JsonObject& parsed = JSONBuffer.parseObject(data); 
    if (!parsed.success()) { //Check for errors in parsing
      Serial.println("Parsing failed");
      delay(5000);
      return;
    }
    Direction = parsed["Direction"];
    Speed=parsed["speed"];
    
    if (Direction=="up")
    {
      Forward();
      Serial.println("Going Forward");
    }
    else if(Direction=="left")
    {
      Left();
      Serial.println("Going Left");
    }
    else if(Direction=="right")
    {
      Right();
      Serial.println("Going Right");

    }
    else if(Direction=="down")
    {
      Backwards();
      Serial.println("Going Back");
    }
    else
    {
      Reset_Motors();
      Serial.println("Stopped");
    }
    




    
    data = "Rover is connected...";
    
    webSocketClient.sendData(data);
    
  } else {
    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }
  
  // wait to fully let the client disconnect
  // delay(400);
  
}

