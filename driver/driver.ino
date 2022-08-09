#include "Driver.h"

//ESC esc = ESC(30);
Solenoid sol = Solenoid(28);
unsigned long pretime_s;
void setup() {
  Serial.begin(115200);
//  esc.attachESC();
  sol.setupSol();
  while (!Serial.available());
//  esc.setupESC();
  while (Serial.available())Serial.read();
  pretime_s = micros();
}

void loop() {
//  esc.drive(1000);
  if (Serial.available()) {
      shoot(30, 2000000);
    }
    while(Serial.available())
      Serial.read();
}


void shoot(uint16_t power, long break_time) {
  if (pretime_s + break_time < micros())
  {
//    esc.drive(1000);
//    delay(50);
    sol.shot(power);
//    esc.drive(1000);
    pretime_s = micros();
  }
}
