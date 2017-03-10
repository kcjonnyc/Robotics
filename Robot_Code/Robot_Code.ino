// Robot Code
// Our robot was built using two servos and three ultrasonic sensors.
// The ultrasonic sensors were placed on the two sides and the front all facing down.
// This particular setup and code allows for the robot to drive along any elevated path
// without falling off. It essentially, drives straight and then adjusts it's position
// if it is about to fall off the edge.

#include <Servo.h>

// create servo objects
Servo leftMotor;
Servo rightMotor;

const int serialPeriod = 250;       // only print to the serial console every 1/4 second
unsigned long timeSerialDelay = 0;

const int loopPeriod = 20;          // a period of 20ms = a frequency of 50Hz
unsigned long timeLoopDelay   = 0;

// specify the trig & echo pins used for the ultrasonic sensors
const int ultrasonic2TrigPin = 8;
const int ultrasonic2EchoPin = 9;
const int ultrasonic3TrigPin = 11;
const int ultrasonic3EchoPin = 10;
const int ultrasonic4TrigPin = 6;
const int ultrasonic4EchoPin = 7;

boolean turnLeft = true;

int ultrasonic2Distance;
int ultrasonic2Duration;
int ultrasonic3Distance;
int ultrasonic3Duration;
int ultrasonic4Distance;
int ultrasonic4Duration;

int timesTurned = 0;

// define the states
#define DRIVE_FORWARD     0
#define TURN_LEFT         1

int state = DRIVE_FORWARD; // 0 = drive forward (DEFAULT), 1 = turn left

void setup()
{
    Serial.begin(9600);
  
    // ultrasonic sensor pin configurations
    pinMode(ultrasonic2TrigPin, OUTPUT);
    pinMode(ultrasonic2EchoPin, INPUT);
    
    pinMode(ultrasonic3TrigPin, OUTPUT);
    pinMode(ultrasonic3EchoPin, INPUT);
    
    pinMode(ultrasonic4TrigPin, OUTPUT);
    pinMode(ultrasonic4EchoPin, INPUT);
    
    leftMotor.attach(13);
    rightMotor.attach(12);
}


void loop()
{
    debugOutput(); // prints debugging messages to the serial console
    
    if(millis() - timeLoopDelay >= loopPeriod)
    {
        readUltrasonicSensors(); // read and store the measured distances
        
        stateMachine();
        
        //timeLoopDelay = millis();
    }
}


void stateMachine()
{
    if(state == DRIVE_FORWARD) // no obstacles detected
    {
        if(ultrasonic2Distance < 10 && ultrasonic2Distance > 0) // if there's something underneath the sensor (note: ultrasonicDistance will be negative for some ultrasonics if there's nothing in range)
        {
          if (ultrasonic3Distance < 10 && ultrasonic3Distance > 0) // space on the side
          {
              // drive forward
              rightMotor.write(180);
              leftMotor.write(0);
          }
          else
          {
              while (!(ultrasonic3Distance < 10 && ultrasonic3Distance > 0))
              {
                  rightMotor.write(180);
                  leftMotor.write(90);
                  readUltrasonicSensors();
                  debugOutput();
              }
          }
              
          if (ultrasonic4Distance < 10 && ultrasonic4Distance > 0) // space on the side
          {
              // drive forward
              rightMotor.write(180);
              leftMotor.write(0);
          }
          else
          {
              while (!(ultrasonic4Distance < 10 && ultrasonic4Distance > 0))
              {
                  rightMotor.write(90);
                  leftMotor.write(0);
                  readUltrasonicSensors();
                  debugOutput();
              }
          }
        }
        else // the platform ends - there's nothing there
        {
            state = TURN_LEFT;
        }
        
    }
    else if(state == TURN_LEFT) // no platform -- turn left
    {
      
        turnLeft = true;
      
        if (!(ultrasonic4Distance < 10 && ultrasonic4Distance > 0)) // space on the side
        {
          turnLeft = false;
        }
        
        if (ultrasonic3Distance < 10 && ultrasonic3Distance > 0) // space on the side
        {
          turnLeft = false;
        }
      
        unsigned long timeToMoveBack = 150; // moves back for have a second
        
        unsigned long turnStartTime = millis(); // save the time that we started turning

        while((millis()-turnStartTime) < timeToMoveBack) // stay in this loop until it has turned for 500ms
        {
            // move back
            rightMotor.write(0);
            leftMotor.write(180);
        }    
        
        delay (500);
      
        if (turnLeft)
        {
            unsigned long timeToTurnLeft = 244; // it takes around 0.244 seconds to turn 20 degrees
        
            turnStartTime = millis(); // save the time that we started turning

            while((millis()-turnStartTime) < timeToTurnLeft) // stay in this loop until timeToTurnLeft has elapsed
            {  
                // turn left
                rightMotor.write(180);
                leftMotor.write(180);
            }  
        /
        else 
        {
            unsigned long timeToTurnRight = 244; // it takes around 0.244 seconds to turn 20 degrees
        
            turnStartTime = millis(); // save the time that we started turning

            while((millis()-turnStartTime) < timeToTurnRight) // stay in this loop until timeToTurnLeft has elapsed
            {  
                // turn left
                rightMotor.write(0);
                leftMotor.write(0);
            }  
        }
        
        state = DRIVE_FORWARD;
    }
}


void readUltrasonicSensors()
{
    // ultrasonic 2
    digitalWrite(ultrasonic2TrigPin, HIGH);
    delayMicroseconds(10);                  // must keep the trig pin high for at least 10us
    digitalWrite(ultrasonic2TrigPin, LOW);
    
    ultrasonic2Duration = pulseIn(ultrasonic2EchoPin, HIGH);
    ultrasonic2Distance = (ultrasonic2Duration/2)/29;
    
    // ultrasonic 3
    digitalWrite(ultrasonic3TrigPin, HIGH);
    delayMicroseconds(10);                  // must keep the trig pin high for at least 10us
    digitalWrite(ultrasonic3TrigPin, LOW);
    
    ultrasonic3Duration = pulseIn(ultrasonic3EchoPin, HIGH);
    ultrasonic3Distance = (ultrasonic3Duration/2)/29;
    
    // ultrasonic 4
    digitalWrite(ultrasonic4TrigPin, HIGH);
    delayMicroseconds(10);                  // must keep the trig pin high for at least 10us
    digitalWrite(ultrasonic4TrigPin, LOW);
    
    ultrasonic4Duration = pulseIn(ultrasonic4EchoPin, HIGH);
    ultrasonic4Distance = (ultrasonic4Duration/2)/29;
}


void debugOutput()
{
    //if((millis() - timeSerialDelay) > serialPeriod)
    //{
        Serial.print("ultrasonic2Distance: ");
        Serial.print(ultrasonic2Distance);
        Serial.print("cm");
        Serial.println();
        
        Serial.print("ultrasonic3Distance: ");
        Serial.print(ultrasonic3Distance);
        Serial.print("cm");
        Serial.println();
        
        Serial.print("ultrasonic4Distance: ");
        Serial.print(ultrasonic4Distance);
        Serial.print("cm");
        Serial.println();
        
        //timeSerialDelay = millis();
    //}
}
