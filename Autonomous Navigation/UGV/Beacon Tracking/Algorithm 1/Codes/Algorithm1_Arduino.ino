#include <HCSR04.h>
#define Echo  7
#define Trig  8
UltraSonicDistanceSensor distanceSensor(Trig, Echo); 
const int Motor_L_F = 2;
const int Motor_L_B = 3;
const int Motor_R_F = 4;
const int Motor_R_B = 5;

void setup() {
  Serial.begin(9600); 
  // put your setup code here, to run once:
  // Define Motor Pin as output
  pinMode(Motor_L_F, OUTPUT);
  pinMode(Motor_L_B, OUTPUT);
  pinMode(Motor_R_F, OUTPUT);
  pinMode(Motor_R_B, OUTPUT);
  pinMode(9, OUTPUT);
  analogWrite(9,100);
}

void loop() 
{
  Stop();
  int Frssi = check_RSSI();
  while (Frssi>114)
  {
    int Frssi = check_RSSI();
  }
  rightTurn();
  int Rrssi = check_RSSI();
  leftTurn();
  leftTurn();
  int Lrssi = check_RSSI();
  int mxOf=maxof(Frssi,Rrssi,Lrssi);
  Serial.print("max = ");
  Serial.println(mxOf);
  for (int i=0;i<mxOf;i++)
  {
    rightTurn();
  }
  int Front_D = distanceSensor.measureDistanceCm();
  Forward();
  if (Front_D<0 || Front_D>50)
  delay(1000);
  else
  delay(20*Front_D);
  Stop();
  delay(500);
}

void rightTurn()
{
  Right();
  delay(400);
  Stop();
  delay(500);
}

void leftTurn()
{
  Left();
  delay(400);
  Stop();
  delay(500);
}



int maxof(int a,int b,int c)
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

int check_RSSI(){
  //char str;
  int rssi=0;
  for(int j=0;j<20;j++)
  {
  while(Serial.available()==0);
  rssi = rssi + Serial.read();
  }
  rssi = rssi / 40;
  Serial.println(rssi);
  return rssi;
}


void Back(){
  // Run Left Motor In Forward Direction
  digitalWrite(Motor_L_F, HIGH);
  digitalWrite(Motor_L_B, LOW);

  //Run Right Motor in Forward Direction
  digitalWrite(Motor_R_F, HIGH);
  digitalWrite(Motor_R_B, LOW);
  
  }

void Right(){
  //Stop Right Motor
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, LOW);

  //Run Left Motor in Forward Direction
  digitalWrite(Motor_L_F, HIGH);
  digitalWrite(Motor_L_B, LOW);
  
  }

void Left(){
  //Stop Left Motor
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, LOW);

  //Run Right Motor in Forward Direction
  digitalWrite(Motor_R_F, HIGH);
  digitalWrite(Motor_R_B, LOW);
  
  }

void Forward(){
  // Run Left Motor In Forward Direction
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, HIGH);

  //Run Right Motor in Forward Direction
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, HIGH);
  
  }

void Stop(){
  // Run Left Motor In Forward Direction
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, LOW);

  //Run Right Motor in Forward Direction
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, LOW);
  
  }
