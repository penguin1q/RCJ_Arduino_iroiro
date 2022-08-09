#ifndef MOTOR_H
#define MOTOR_H

class Motor {
  public:
    Motor();
    Motor(byte pin[]);
    void setupPin();
    void drive(int data, byte brake);

  private:
    byte pin[2];
};

class Omni_4wheel{
  public:
    Omni_4wheel(byte pin0[], byte pin1[], byte pin2[], byte pin3[]);
    Omni_4wheel(byte pins[][2]);
    void setupPins();
    void control(int16_t data[]);
    void setBias(float motor0 = 0, float motor1 = 0, float motor2 = 0, float motor3 = 0);
  private:
    int max_abs(int data[], int num);
  private:
    float bias[4]={1};
    Motor motor[4];
};

#endif
