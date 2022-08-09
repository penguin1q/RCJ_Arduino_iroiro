#include "Motor.h"

const byte pins[][2] = {{4, 5}, {2, 3}, {9, 8}, {7, 6}};
Omni_4wheel omuni = Omni_4wheel(pins);

void setup() {
  // put your setup code here, to run once:
  omuni.setBias(0.99, 1.0, 1.0, 1.0);
  omuni.setupPins();
}

void loop() {
  // put your main code here, to run repeatedly:
 int16_t data[] = {0,0,0};
 omuni.control(data);
}
