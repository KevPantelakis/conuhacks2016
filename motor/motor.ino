const int controlPin1 = 2; // connected to pin 7 on the H-bridge
const int controlPin2 = 3; // connected to pin 2 on the H-bridge
const int enablePin = 9;   // connected to pin 1 on the H-bridge

int motorEnabled = 0; // Turns the motor on/off
int motorSpeed = 0; // speed of the motor
int motorDirection = 1; // current direction of the motor

void setup() {
  // intialize the inputs and outputs
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // pull the enable pin LOW to start
  digitalWrite(enablePin, LOW);
}

void loop() {
  // read the value of the on/off switch
    digitalWrite(controlPin1, HIGH);
    digitalWrite(controlPin2, LOW);

    for(int i = 0; i < 100; i++){
      analogWrite(enablePin, i);
      delay(15);
    }
    for(int i = 100; i > 0; i--){
      analogWrite(enablePin, i);
      delay(15);
    }

    delay(2000);
    
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);

    for(int i = 0; i < 100; i++){
      analogWrite(enablePin, i);
      delay(15);
    }
    for(int i = 100; i > 0; i--){
      analogWrite(enablePin, i);
      delay(15);
    }

    delay(2000);

}




