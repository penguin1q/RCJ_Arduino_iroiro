#ifndef ESC_H
#define ESC_H
#include <Servo.h>

class ESC
{
  public:
    ESC(byte pin, int minval=1000, int maxal=2000);
    void setupESC();
    void attachESC();
    void drive(int power);
  private:
    unsigned long pretime;
    int minval, maxval;
    byte pin;
    Servo motor;
};

class Solenoid
{
  public:
    Solenoid(byte pin);
    void setupSol();
    void shot(int power);
  private:
    byte pin;
};
#endif
