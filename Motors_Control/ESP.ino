#include <WiFi.h>
#include "ArduinoJson.h"
#include <WebSocketClient.h>

const char* ssid = "Skyguy";
const char* password = "TheForce";
char path[] = "/";
char host[] = "192.168.73.160:8082";
String IP ="192.168.73.160";
WebSocketClient webSocketClient;
String Direction;
// Use WiFiClient class to create TCP connections
WiFiClient client;


int motor1Pin1=18;
int motor1Pin2=5;
int Enable1Pin=19;

int motor2Pin1=17;
int motor2Pin2=16;
int Enable2Pin=4;

const int freq= 30000;
const int pwmChannel=0;
const int resolution=8;
int Speed=155;


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
  if (client.connect("192.168.73.160", 8082)) {
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
  ledcWrite(pwmChannel, Speed);
  digitalWrite(motor1Pin1,HIGH);
  digitalWrite(motor1Pin2,LOW);
  digitalWrite(motor2Pin1,HIGH);
  digitalWrite(motor2Pin2,LOW);

}

void Right()
{
  ledcWrite(pwmChannel, Speed);
  digitalWrite(motor1Pin1,HIGH);
  digitalWrite(motor1Pin2,LOW);
  digitalWrite(motor2Pin1,LOW);
  digitalWrite(motor2Pin2,HIGH);
}

void Left()
{
  ledcWrite(pwmChannel, Speed);
  digitalWrite(motor1Pin1,LOW);
  digitalWrite(motor1Pin2,HIGH);
  digitalWrite(motor2Pin1,HIGH);
  digitalWrite(motor2Pin2,LOW);
}

void Backward()
{
  ledcWrite(pwmChannel, Speed);
  digitalWrite(motor1Pin1,LOW);
  digitalWrite(motor1Pin2,HIGH);
  digitalWrite(motor2Pin1,LOW);
  digitalWrite(motor2Pin2,HIGH);

}

void processReceivedJSON(String jsonStr) {
  // Create a JSON document to parse the data
  DynamicJsonDocument doc(256); // Adjust the size as per your needs

  // Parse the JSON string
  deserializeJson(doc, jsonStr);

  // Check for parsing errors


  // Access the JSON data and assign to golobal variables
  Direction = doc["direction"].as<String>();
  Speed = doc["speed"].as<int>();
  Speed= map(Speed,0,100,0,255);
  

  // Process the parsed data as needed
  Serial.println("Received JSON data:");
  Serial.print("key1: ");
  Serial.print(Direction);
  Serial.print(", key2: ");
  Serial.println(Speed);
  
}




void loop() {
  String data;
 

  if (client.connected()) {
    
    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
    }
    

    processReceivedJSON(data);
    
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
      Backward();
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
  delay(400);
  
}

// void loop()
// {
//   // Move forward
//   Serial.println("Forward");
//   Forward();
//   delay(5000);
//   Serial.println("Righty");
//   Righty();
//   delay(5000);
// }