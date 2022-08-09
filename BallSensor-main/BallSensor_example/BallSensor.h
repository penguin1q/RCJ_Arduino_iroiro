#ifndef _BallSensor_h
#define _BallSensor_h

#define BALL_CATCH_PIN A12
#define eeprom_offset 100
#define ball_threshold 130

class BallSensor {

  public:
    bool getHold();

};

#endif
