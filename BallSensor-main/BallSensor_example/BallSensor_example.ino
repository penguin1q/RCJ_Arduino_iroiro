#include "BallSensor.h"

BallSensor BS;

void setup() {
  Serial.begin(9600);
}

void loop() {
  bool bs = false;
  bs = BS.getHold();
  Serial.print("ボールセンサ: ");  Serial.println(bs);
}
