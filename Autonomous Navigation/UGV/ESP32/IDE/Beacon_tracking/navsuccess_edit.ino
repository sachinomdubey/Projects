#include <WiFi.h>
#include "analogWrite.h"

//Setting up WiFi server and Client
String  ClientRequest;
WiFiServer server(80);
WiFiClient client;
String myresultat;
String ReadIncomingRequest(){
  while(client.available()) {
    ClientRequest = (client.readStringUntil('\r'));
    if ((ClientRequest.indexOf("HTTP/1.1")>0)&&(ClientRequest.indexOf("/favicon.ico")<0)){
      myresultat = ClientRequest;
    }
  }
  return myresultat;
}

//Defining pins for motor1 and motor2 direction control
const int Motor_L_F = 16;
const int Motor_L_B = 17;
const int Motor_R_F = 18;
const int Motor_R_B = 19;

//Connecting to the beacon in order to read RSSI values
void setup()
{
  ClientRequest = "";
  Serial.begin(115200);
  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin("Sachin","sachin1234"); //Change this details as per your android phone
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
    Serial.print("..");
  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));
  server.begin();
  
  pinMode(14, OUTPUT);  //Speed control output pins
  pinMode(15, OUTPUT);
  
  pinMode(Motor_L_F, OUTPUT); //Directional control output pins
  pinMode(Motor_L_B, OUTPUT);
  pinMode(Motor_R_F, OUTPUT);
  pinMode(Motor_R_B, OUTPUT);
}


void loop()
{
  analogWrite(14,170);                  //Speed control of motor-1
  analogWrite(15,170);                  //Speed control of motor-2
  int Frssi = check_RSSI();             //Read the RSSI value in "Forward" direction
  while (Frssi>114)                     //Setting threshold for stopping the UGV on reaching close to beacon
  {
    int Frssi = check_RSSI();
  }
  rightTurn();                          // Turn Right in-place 
  int Rrssi = check_RSSI();             // Read the RSSI value in "Right" direction
  leftTurn();                           //Turn Left in-Place to get back to "Forward" direction
  leftTurn();                           //Turn left in-place to turn to "Left" direction
  int Lrssi = check_RSSI();             //Read the RSSI value in "Left" direction
  int mxOf=maxof(Frssi,Rrssi,Lrssi);    //Find the max value amoung the RSSI values read in the three directions
  //Serial.print("max = ");
  //Serial.println(mxOf);

  //Turn towards the direction of maximum RSSI and move towards that direction
  for (int i=0;i<mxOf;i++)
  {
    rightTurn();
  }
  Forward();
  delay(700);
  Stop();
  delay(300);
}

int maxof(int a,int b,int c)  //Function to find max amoung the three RSSI values
{
  int mx = 1;
  if (b>a)
  {
    mx = 2;
    if (c>=b)
    {
      mx = 0;   
    }
  }
 else if (c>=a)
 {
  mx=0;
 }
 return mx;
}

int check_RSSI(){               // Function to read RSSI values from the android phone (beacon)
  int rssi=0;
  int x;
  for(int j=0;j<20;j++)
  {
   x=WiFi.RSSI();
  rssi = rssi + x;
  }
  rssi = rssi / 40;         //Averaging over 20 points for stable readings
  Serial2.write(rssi);
  Serial.println(rssi);
  return rssi;
}

void rightTurn(){               //Turn right 90 degree in-place
  Right();
  delay(300);
  Stop();
  delay(400);
}

void leftTurn(){                //Turn left 90 degree in-place
  Left();
  delay(300);
  Stop();
  delay(400);
}

void Forward(){                   //Move forward
  digitalWrite(Motor_L_F, HIGH);
  digitalWrite(Motor_L_B, LOW);
  digitalWrite(Motor_R_F, HIGH);
  digitalWrite(Motor_R_B, LOW);
  
  }

void Right(){                     //Move Right
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, LOW);
  digitalWrite(Motor_L_F, HIGH);
  digitalWrite(Motor_L_B, LOW);
}

void Left(){                      //Move Left
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, LOW);
  digitalWrite(Motor_R_F, HIGH);
  digitalWrite(Motor_R_B, LOW);
}

void Back(){                      //Move Back
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, HIGH);
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, HIGH);
}

void Stop(){                      //Stop
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, LOW);
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, LOW);
}
