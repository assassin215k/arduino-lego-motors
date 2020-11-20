#include <Stepper.h>

const int stepsPerRevolution = 2048; // шагов за один оборот

Stepper myStepper(stepsPerRevolution, 2, 4, 3, 5); 

void setup() {
  myStepper.setSpeed(16); // скорость 5 об/минуту
}

void loop() {
  myStepper.setSpeed(1500);
  //myStepper.step(stepsPerRevolution); // 32 шага в одном направлении
  //delay(500);
 
  //Serial.println("counterclockwise");
  //myStepper.setSpeed(1000);
  //myStepper.step(-stepsPerRevolution); // 32 шага в обратную сторону
  //delay(500); 

  //myStepper.step(50);
}
