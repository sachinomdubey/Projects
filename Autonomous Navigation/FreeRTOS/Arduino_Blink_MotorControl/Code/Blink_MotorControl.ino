#include <Arduino_FreeRTOS.h>

// define two tasks for Blink & Motor_Control
void TaskBlink( void *pvParameters );
void TaskMotorControl( void *pvParameters );

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  // Now set up two tasks to run independently.
  xTaskCreate(
    TaskBlink
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskMotorControl
    ,  "MotorControl"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*---------------------- Tasks ---------------------*/

void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}

void TaskMotorControl(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  int led = 9;           // The motors speed control pin
  int brightness = 0;    // decides how fast the Motor will rotate.
  int fadeAmount = 5;    // decides by how many points to speedup/slow-down the Motor.
  pinMode(led, OUTPUT);    // Motor speed control pin (9)
  pinMode(4, OUTPUT);    // Motor direction control pins (4 and 5)
  pinMode(5, OUTPUT);
  
  for (;;)
  {
    analogWrite(led, brightness);

    // change the speed for next time through the loop:
    brightness = brightness + fadeAmount;
    
    // Start slowing down or speeding up the motor at points where speed is maximum or minimum respectively
    if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;

    // wait for 30 milliseconds to see the slowing/speeding effect
    delay(30);
    }
  }
}
