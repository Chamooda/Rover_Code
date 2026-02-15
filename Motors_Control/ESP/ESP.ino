#include <WiFi.h>
#include "ArduinoJson.h"
#include <WebSocketClient.h>
#include <DHT.h>
#include <ESP32Servo.h>

#define DHTPIN 0     // Digital pin connected to the DHT sensor
const char* ssid = "Skyguy";
const char* password = "TheForce";
char path[] = "/";
char host[] = "192.168.27.104:8082";
String IP ="192.168.27.104";
WebSocketClient webSocketClient;
String Direction;
// Use WiFiClient class to create TCP connections
WiFiClient client;

#define RX 1
#define TX 3

int motor1Pin1=27;
int motor1Pin2=26;
int Enable1=32;

int motor2Pin1=14;
int motor2Pin2=22;


int motor3Pin1=5;
int motor3Pin2=4;
int Enable2=18;

int motor4Pin1=17;
int motor4Pin2=16;


const int freq= 30000;
const int pwmChannel=0;
const int resolution=8;
int Speed=155;

const int mq7Pin = 19; 
// const int mq4Pin = 0;
// const int irsensor = 0;

// #define servoPin 23

// #define DHTTYPE DHT11     // DHT 11
// int Angle=0;
// DHT dht(DHTPIN, DHTTYPE);
//MQ7 Calibration 
// float sensor_volt;
// float RS_gas; 
// float R0;
int R2 = 2000;


//MQ4 Calibration
// const int R_0 = 945; //Change this to your own R0 measurements


// current temperature & humidity, updated in loop()
// float t = 0.0;
// float h = 0.0;
int Angle=0;


  Servo myServo;
void setup() {
  Serial.begin(115200);
  // dht.begin();
  Serial2.begin(9600, SERIAL_8N1, RX, TX);
  delay(10);

  // myServo.attach(servoPin);
  // myServo.write(0);

  //Setting up motor driver and controls
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(Enable1, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  

  pinMode(motor3Pin1, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
  pinMode(Enable2, OUTPUT);

  pinMode(motor4Pin1, OUTPUT);
  pinMode(motor4Pin2, OUTPUT);
  


  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(Enable1, pwmChannel);
  ledcAttachPin(Enable2, pwmChannel);
   

  pinMode(mq7Pin, INPUT);
  // pinMode(mq4Pin, INPUT);
  // pinMode(servoPin,OUTPUT);
  // pinMode(DHTPIN,INPUT);
    


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
  if (client.connect("192.168.27.104", 8082)) {
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

  digitalWrite(motor3Pin1,LOW);
  digitalWrite(motor3Pin2,LOW);

  digitalWrite(motor4Pin1,LOW);
  digitalWrite(motor4Pin2,LOW);  

}

void Right()
{
  ledcWrite(pwmChannel, Speed);

  digitalWrite(motor1Pin1,LOW);
  digitalWrite(motor1Pin2,HIGH);

  digitalWrite(motor2Pin1,HIGH);
  digitalWrite(motor2Pin2,LOW);

  digitalWrite(motor3Pin1,HIGH);
  digitalWrite(motor3Pin2,LOW);

  digitalWrite(motor4Pin1,LOW);
  digitalWrite(motor4Pin2,HIGH);

}

void Backward()
{
  ledcWrite(pwmChannel, Speed);

  digitalWrite(motor1Pin1,LOW);
  digitalWrite(motor1Pin2,HIGH);
  digitalWrite(motor2Pin1,LOW);
  digitalWrite(motor2Pin2,HIGH);

  digitalWrite(motor3Pin1,LOW);
  digitalWrite(motor3Pin2,HIGH);
  digitalWrite(motor4Pin1,LOW);
  digitalWrite(motor4Pin2,HIGH); 
}

void Forward()
{
  ledcWrite(pwmChannel, Speed);

  digitalWrite(motor1Pin1,HIGH);
  digitalWrite(motor1Pin2,LOW);

  digitalWrite(motor2Pin1,HIGH);
  digitalWrite(motor2Pin2,LOW);

  digitalWrite(motor3Pin1,HIGH);
  digitalWrite(motor3Pin2,LOW);

  digitalWrite(motor4Pin1,HIGH);
  digitalWrite(motor4Pin2,LOW); 
}

void Left()
{
  ledcWrite(pwmChannel, Speed);

  digitalWrite(motor1Pin1,HIGH);
  digitalWrite(motor1Pin2,LOW);

  digitalWrite(motor2Pin1,LOW);
  digitalWrite(motor2Pin2,HIGH);

  digitalWrite(motor3Pin1,LOW);
  digitalWrite(motor3Pin2,HIGH);

  digitalWrite(motor4Pin1,HIGH);
  digitalWrite(motor4Pin2,LOW); 

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
  Angle= doc["Angle"].as<int>();

  // Process the parsed data as needed
  Serial.println("Received JSON data:");
  Serial.print("key1: ");
  Serial.print(Direction);
  Serial.print(", key2: ");
  Serial.println(Speed);


}


void loop() {
  String data;
  String data_send;
  int pos;
  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myServo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myServo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  

  // int IR=analogRead(irsensor);
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
    

    // Read temperature as Celsius (the default)
    // float newT = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float newT = dht.readTemperature(true);
    // Read Humidity
    // float newH = dht.readHumidity();



    // Read sensor values
    int mq7Value = analogRead(mq7Pin);
    Serial.println(mq7Value);
    
    data_send=Serial2.readString();
    // data_send="{\"mq7\":287,\"mq4\":400,\"Temp\":23.9,\"Humid\":31}";
    Serial.println("\n\n");
    // Serial2.println(Angle);
    // int mq4Value = analogRead(mq4Pin);
    // float v_o = mq4Value * 5 / 1023; // convert reading to volts
    // float R_S = (5-v_o) * 1000 / v_o; // apply formula for getting RS
    // float PPM = pow(R_S/R_0,-2.95) * 1000; //apply formula for getting PPM

    // Create a JSON object to store the sensor data
    // DynamicJsonDocument sensorData(128);
    // sensorData["mq7"] = mq7Value;
    // sensorData["mq4"] = PPM;
    // sensorData["temperature"] = newT;
    // sensorData["Humidity"] = newH;
    // Convert the JSON object to a string
    // serializeJson(sensorData, data_send);

    // Send sensor data over the WebSocket

    webSocketClient.sendData(data_send);
    Serial.println(data_send);
    // Delay or adjust as needed
    
  } else {
    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }
  
  // wait to fully let the client disconnect
  
  delay(100);
}