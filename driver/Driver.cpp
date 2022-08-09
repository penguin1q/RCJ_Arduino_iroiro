#include <Arduino.h>
#include "Driver.h"
#include <Servo.h>

ESC::ESC(byte pin, int minval = 1000, int maxal = 2000) {
  
  this->minval = minval;
  this->maxval = maxval;
  this->pin = pin;
  pretime = micros();
}

void ESC::attachESC()
{
  motor.attach(pin);
}

void ESC::setupESC()
{
  motor.writeMicroseconds(maxval);
  delay(3000);
  motor.writeMicroseconds(minval);
  delay(3000);
}

void ESC::drive(int power)
{
  //if (pretime + 1000000<micros()) {
//    if (power > maxval) power = 2000;
//    if (power < minval) power = 1000;
    motor.writeMicroseconds(power);
    pretime = micros();
  //}
}


Solenoid::Solenoid(byte pin)
{
  this->pin = pin;
}

void Solenoid::setupSol() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  
}

void Solenoid::shot(int power)
{
  digitalWrite(pin, HIGH);
  delay(power);
  digitalWrite(pin, LOW);
}
