#include "Driver.h"

ESC esc = ESC(30);
Solenoid sol = Solenoid(28);
unsigned long pretime_s;

void setup() {
  Serial.begin(115200);
  esc.attachESC();
  sol.setupSol();
  while (!Serial.available());
  esc.setupESC();
  pretime_s = micros();
}

void loop() {
  esc.drive(1300);
  while (Serial.available())
    Serial.read();
  while (!Serial.available());
  shoot(80, 3000000);

}

void shoot(uint16_t power, long break_time) {
  if (pretime_s + break_time < micros())
  {
    esc.drive(1080);
    delay(70);
    sol.shot(power);
    esc.drive(1300);
    pretime_s = micros();
  }
}
