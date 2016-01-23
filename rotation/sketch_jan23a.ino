
#include <Servo.h>

  Servo rotator; 

  int pos; 
  int angl; 
  int poten; 
  
void setup() {
   rotator.attach(6);
   Serial.begin(9600); 
   
}

void loop() {
  
  poten = analogRead(A5); 
  angl = map(poten, 0, 1023, 0 , 179);
   
  rotator.write(angl);   
  delay(15);
 
   

}
