#include "BallSensor.h"
#include "Arduino.h"

bool BallSensor::getHold(void) {
  int raw_value = 0;
  bool ball_sensor = false;
  raw_value = analogRead(BALL_CATCH_PIN);
  //Serial.print("raw_value: ");  Serial.println(raw_value);
  if (raw_value > ball_threshold) {
    ball_sensor = true;
  }
  else {
    ball_sensor = false;
  }
  //Serial.print("ball_sensor: ");  Serial.println(ball_sensor);
  return ball_sensor;
}
