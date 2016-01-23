
#include <Servo.h>

  Servo rotator; 

  int pos; 
  
void setup() {
   rotator.attach(6);
}

void loop() {
      
    for (pos = (180*analogRead(A5))/1023); pos >= 0; pos -= 1) { 
    rotator.write(pos);             
    delay(15);                    
  }
   

}
