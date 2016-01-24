#include <Servo.h>


//Motor speed limit
#define baseSpeed 50


//Servo declaration
Servo servoDir;
Servo servoCam;
#define servoDirPin 12
#define servoCamPin 13
int posServoDir;
int posServoCam;  

// motor declaration
#define motorCtrlPin1 2 // connected to pin 2 on the H-bridge
#define motorCtrlPin2 3 // connected to pin 3 on the H-bridge
#define motorEnPin 9   // connected to pin 9 on the H-bridge

int motorEnabled = 0; // Turns the motor on/off
int motorActualSpeed = 0; // speed of the motor
bool motorDirection = 1; // current direction of the motor
int motorSpeedMode = 0;

int serialSM = 0;
int serialAng = 0;
int deltaSpeed = 0;


void setup() {
  Serial.begin(9600);
  servoDir.attach(servoDirPin);
  servoCam.attach(servoCamPin);
  
  pinMode(motorCtrlPin1, OUTPUT);
  pinMode(motorCtrlPin2, OUTPUT);
  pinMode(motorEnPin, OUTPUT);

  digitalWrite(motorEnPin, LOW);

}

void loop() {
  
  if(Serial.available()){
    serialAng = Serial.read();
    serialSM = Serial.read();
  }
  
  if(serialSM < 4 && serialSM > -4){
    if(serialSM != motorSpeedMode){
      if (serialSM < 0){
        motorDirection = 0;
        motorSpeedMode = -serialSM;
      }
      else{
        motorDirection = 1;
        motorSpeedMode = serialSM;
      } 
      deltaSpeed = motorActualSpeed - motorSpeedMode*baseSpeed;
      for (int i = 0; i < abs(deltaSpeed); i++){
        motorActualSpeed += (deltaSpeed * 3)/abs(deltaSpeed);
        analogWrite(motorEnPin, motorActualSpeed);  
      }
    }
  }
  
  if(motorDirection){
    digitalWrite(motorCtrlPin1, HIGH);
    digitalWrite(motorCtrlPin2, LOW);
  }else{
    digitalWrite(motorCtrlPin1, LOW);
    digitalWrite(motorCtrlPin2, HIGH);  
  }
  
  
  if(serialAng < 136 && serialAng > 44){
    if(serialAng != posServoDir){
      posServoDir = serialAng;
      servoDir.write(posServoDir); 
    } 
  }
  
  
  
  
}
