#define ip1 14    // Input from receiver channel 2    
#define ip2 15    // Input from receiver channel 4  
const int Motor_L_F = 16; // Motor 1 terminal 1
const int Motor_L_B = 17; // Motor 1 terminal 2
const int Motor_R_F = 18; // Motor 2 terminal 1
const int Motor_R_B = 19; // Motor 2 terminal 2
int ch2,ch4;  //Variables to store input from receiver

void setup()
{
  Serial.begin(9600); // Serial communication for printing and observing value of variable.
  
  // Defining pin modes.
  pinMode(ip1, INPUT);  
  pinMode(ip2, INPUT);  
  pinMode(Motor_L_F, OUTPUT); 
  pinMode(Motor_L_B, OUTPUT); 
  pinMode(Motor_R_F, OUTPUT); 
  pinMode(Motor_R_B, OUTPUT); 
}

void loop()
{
  ch2 = pulseIn(ip1, HIGH); //Reading the ON duration of PWM signal on channel-2 receiver
  ch4 = pulseIn(ip2, HIGH); //Reading the ON duration of PWM signal on channel-4 receiver
  
  if((ch2>1400 && ch2<1600) && (ch4>1400 && ch4<1600)) //Depending upon the values of ch2 and ch4, necessary navigation command are sent to motor driver IC
    Stop();   
  if (ch2 > 1600) 
    Forward(); 
  else
  {
    if(ch2<1400)
      Back();
    else
    {
      if (ch4 >1600)
        Left();
      else
      {
        if(ch4<1400)
          Right(); 
      }
    }
  }
}

void Stop() //Stop function
{
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, LOW);
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, LOW);
}
void Forward()  //Forward function
{
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, HIGH);
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, HIGH);
}
void Back() //Back function
{
  digitalWrite(Motor_L_F, HIGH);
  digitalWrite(Motor_L_B, LOW);
  digitalWrite(Motor_R_F, HIGH);
  digitalWrite(Motor_R_B, LOW);
}
void Left() //Left function
{
  digitalWrite(Motor_L_F, LOW);
  digitalWrite(Motor_L_B, HIGH);
  digitalWrite(Motor_R_F, HIGH);
  digitalWrite(Motor_R_B, LOW);
}
void Right()  //Right function
{
  digitalWrite(Motor_L_F, HIGH);
  digitalWrite(Motor_L_B, LOW);
  digitalWrite(Motor_R_F, LOW);
  digitalWrite(Motor_R_B, HIGH);
}
