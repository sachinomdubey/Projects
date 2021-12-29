#include <WiFi.h>
#include <WiFiClient.h> 
#include <WebServer.h>

//Declaring pins for motor1 and motor2 direction control
const int Motor_L_F = 16;
const int Motor_L_B = 17;
const int Motor_R_F = 18;
const int Motor_R_B = 19;

WebServer server(80); //Initializing ESP32 as a web server

void setup() {
  Serial.begin(115200);
  
  //Defining the motor directional control pins as output pins
  pinMode(Motor_L_F, OUTPUT);
  pinMode(Motor_L_B, OUTPUT);
  pinMode(Motor_R_F, OUTPUT);
  pinMode(Motor_R_B, OUTPUT);
  
  Serial.println("START");
  WiFi.begin("Sachin","sachin1234");  //Note: This line should be modified to the wifi hotspot SSID/Password of the android phone in use.
  
  // Connecting WiFi
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    Serial.print("..");
  }
  // Starting WEB-server
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));

  server.on ( "/", HTTP_handleRoot );
  server.onNotFound ( HTTP_handleRoot );
  server.begin();    
     
}

void loop() {
  server.handleClient();
  delay(50);
}

void HTTP_handleRoot(void) {
  String ani;
  ani=server.arg("State");
  if (ani == "forward")
  {
    Serial.print("Forward");
    Forward();
  }
  else if (ani == "back")
  {
    Serial.print("back");
    Back();
  }
  else if (ani== "right")
  {
    Serial.print("right");
    Right();
  }
  else if (ani=="left")
  {
    Serial.print("left");
    Left();
  }
  else 
  {
    Serial.print("stop");
    Stop();
  }
}
 
void Forward() { //Move Forward command
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, HIGH);
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, HIGH);
}

void Right() {  //Move Right command
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, HIGH);
  digitalWrite(Motor_R_F, HIGH);
  digitalWrite(Motor_R_B, LOW);
}

void Left() { //Move Left command
  digitalWrite(Motor_L_F, HIGH);
  digitalWrite(Motor_L_B, LOW);
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, HIGH);
}

void Back() { //Move Back command
  digitalWrite(Motor_L_F, HIGH);
  digitalWrite(Motor_L_B, LOW);
  digitalWrite(Motor_R_F, HIGH);
  digitalWrite(Motor_R_B, LOW); 
}

void Stop() { //Move Stop command
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, LOW);
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, LOW);
}
  
