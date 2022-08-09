#include "LineSensor.h"
#include "Motor.h"
#include "Driver.h"

#define SERIAL_DATA_LENGTH 10
#define SERIAL_SEND_DATA_LENGTH 5
#define SERIAL_TWELITE_DATA_LENGTH 7


typedef union {
  byte receive_byte[SERIAL_DATA_LENGTH];
  struct {
    int16_t out[3];
    uint8_t abs_pos[2];
    byte bits;
    byte bit2;
  } data;
} Packet;


typedef union {
  byte send_byte[SERIAL_SEND_DATA_LENGTH];
  struct {
    uint8_t abs_pos[2];
    uint8_t ball_pos[2];
    byte bits;
  } data;
} Tx_Packet;

typedef union {
  byte byte_datas[SERIAL_TWELITE_DATA_LENGTH];
  struct {
    uint8_t abs_pos[2];
    uint8_t ball_pos[2];
    byte bits[3];
  } data;
} TW_Packet;

Packet packet = {0};
Tx_Packet tx_packet = {1, 2, 3, 4, 5};
TW_Packet tw_rx = {0};
TW_Packet tw_tx = {0};
const byte pins[][2] = {{4, 5}, {2, 3}, {9, 8}, {7, 6}};
Omni_4wheel omuni = Omni_4wheel(pins);
unsigned long pretime;
unsigned long pretime_s;
LineSensor LS = LineSensor(false);
ESC esc = ESC(30);
Solenoid sol = Solenoid(28);


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  esc.attachESC();
  LS.setThreshold();//閾値設定
  LS.printThreshold();//閾値表示
  sol.setupSol();

  //delay(5000);//l

  LS.turnOnLed();//led点灯
  //omuni.setBias( 1.010, 0.980, 0.090,1.010);
  while (!Serial.available());
  esc.setupESC();
  omuni.setBias(1.00, 0.990, 0.990, 0.990);
  omuni.setupPins();
  pretime = micros();
  pretime_s = micros();

}

void loop() {
  int lx = 0, ly = 0;
  //  jetsonからのシリアル入力
  while (Serial.available() > SERIAL_DATA_LENGTH)
    if ('A' == Serial.read())
      for (int i = 0; i < SERIAL_DATA_LENGTH; i++) packet.receive_byte[i] = Serial.read();

  //  tweliteからのシリアル入力
  while (Serial1.available() > SERIAL_TWELITE_DATA_LENGTH)
    if ('A' == Serial1.read())
      for (int i = 0; i < SERIAL_DATA_LENGTH; i++) tw_rx.byte_datas[i] = Serial1.read();

  //  tweliteから受け取ったデータをjetsonへの送信パケットに代入
  for (int i = 0; i < SERIAL_SEND_DATA_LENGTH; i++)
    tx_packet.send_byte[i] = tw_rx.byte_datas[i];

  //  jetsonから受け取ったデータをtweliteへの送信パケットに代入
  for (int i = 0; i < 2; i++) {
    tw_tx.data.abs_pos[i] = packet.data.abs_pos[i];
    tw_tx.data.bits[0] = packet.data.bit2;
  }

  /*for test!!!!!!!!*/
  //for (int i = 0; i < SERIAL_SEND_DATA_LENGTH; i++)
   // tx_packet.send_byte[i] = tw_tx.byte_datas[i];

  //  jetsonから動作命令が来ていれば実行
  if (packet.data.bits & 0b00000001)
  {
    LS.getLine(&lx, &ly);
    if (lx + ly)
    {
      packet.data.out[0] = lx;
      packet.data.out[1] = ly;
      packet.data.out[2] = 0;
      //    packet.data.out[0] = -255;
      //    packet.data.out[2] = 0;
    }

    //  ドリブラ回転
    esc.drive(1300);

    //  jetsonとtweliteにデータ送信
    if (pretime + 8000 < micros()) {
      Serial.write('A');
      for (int i = 0; i < SERIAL_SEND_DATA_LENGTH; i++)Serial.write(tx_packet.send_byte[i]);

      Serial1.write('A');
      tw_tx.data.bits[2] = 0x45;
      for (int i = 0; i < SERIAL_TWELITE_DATA_LENGTH; i++)Serial1.write(tw_tx.byte_datas[i]);
      pretime = micros();
    }

    //  モーター出力
    omuni.control(packet.data.out, 0);

    //  キッカー作動
    if (packet.data.bits & 0b00000010) {
      int16_t zeros[] = {0, 0, 0};
      omuni.control(zeros, 1);
      shoot(50, 3000000);
    }
  }
  else
  {
    int16_t zeros[] = {0, 0, 0};
    omuni.control(zeros,0);
    esc.drive(1000);
    // jetsonから128が送られてきた場合、escリセット
    if (packet.data.bits == 0b10000000)
      restart_esc();
  }
}


void shoot(uint16_t power, long break_time) {
  if (pretime_s + break_time < micros())
  {
    esc.drive(1200);
    delay(50);
    sol.shot(power);
    esc.drive(1300);
    pretime_s = micros();
  }
}

void restart_esc() {
  //esc.setupESC();
}
