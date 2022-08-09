#include <Arduino.h>
#include "Motor.h"

Motor::Motor() {
}

Motor::Motor(byte pin[])
{
  for (byte i = 0; i < 2; i++)
    this->pin[i] = pin[i];
}

void Motor::setupPin()
{
  for (byte i = 0; i < 2; i++)
    pinMode(pin[i], OUTPUT);
}

void Motor::drive(int data, byte brake)
{
  int value = 0;
  if (brake != 0) value = 255;
  analogWrite(pin[(data < 0 ? 1 : 0)], (abs(data) & 255) | value);
  analogWrite(pin[(data >= 0 ? 1 : 0)], 0 | value );
}

Omni_4wheel::Omni_4wheel(byte pin0[], byte pin1[], byte pin2[], byte pin3[])
{
  motor[0] = Motor(pin0);
  motor[1] = Motor(pin1);
  motor[2] = Motor(pin2);
  motor[3] = Motor(pin3);
}

Omni_4wheel::Omni_4wheel(byte pins[][2])
{
  motor[0] = Motor(pins[0]);
  motor[1] = Motor(pins[1]);
  motor[2] = Motor(pins[2]);
  motor[3] = Motor(pins[3]);
}

void Omni_4wheel::setupPins()
{
  for (byte i = 0; i < 4; i++)
    motor[i].setupPin();
}

int Omni_4wheel::max_abs(int data[], int num)
{
  int maxdata = 0;
  int data_t;
  for (int i = 0; i < num ; i++)
  {
    data_t = abs(data[i]);
    maxdata = max(data_t, maxdata);
  }
  return maxdata;
}

void Omni_4wheel::control(int16_t data[], byte brake)
{
  float gain_th = 0.7;
  int throttle[4] = {0};
  bool rot = abs(data[2]) < 10;
  float k = rot ? 1 : 0.3;
  float d = rot ? 1 : 1;
  throttle[0] = (( data[0] * gain_th + data[1] * gain_th) * d + data[2] * k) * bias[0];
  throttle[1] = ((-data[0] * gain_th + data[1] * gain_th) * d + data[2] * k) * bias[1];
  throttle[2] = ((-data[0] * gain_th - data[1] * gain_th) * d + data[2] * k) * bias[2];
  throttle[3] = (( data[0] * gain_th - data[1] * gain_th) * d + data[2] * k) * bias[3];

  int flow = 0;
  for (int i = 0; i < 4; i++)
    flow += (throttle[i] > 255 || throttle[i] < -255) ? 1 : 0;

  if (flow) {
    int max_val = max_abs(throttle, 4);
    for (int i = 0; i < 4; i++)
    {
      throttle[i] = (int)(throttle[i] * 255 / max_val);
    }
  }
  
  for (int i = 0; i < 4; i++)
    motor[i].drive(throttle[i], brake);
}

void Omni_4wheel::setBias(float motor0 = 0, float motor1 = 0, float motor2 = 0, float motor3 = 0)
{
  bias[0] = motor0;
  bias[1] = motor1;
  bias[2] = motor2;
  bias[3] = motor3;
}
