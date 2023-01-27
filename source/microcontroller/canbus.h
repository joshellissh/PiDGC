 #ifndef CANBUS_H
#define CANBUS_H

#include <FlexCAN_T4.h>
#include "values.h"

extern FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
extern Values values;

void canSendOBD2(uint8_t activeByte){
  CAN_message_t msgTx;
  msgTx.len = 3; // number of bytes in request
  msgTx.id = 0x7E0;
  msgTx.buf[0] = 0x02; // Number of bytes in request
  msgTx.buf[1] = 0x01; // Mode of request (0x01 = OBD2)
  msgTx.buf[2] = activeByte;
  msgTx.buf[3] = 0x00;
  msgTx.buf[4] = 0x00;
  msgTx.buf[5] = 0x00; 
  msgTx.buf[6] = 0x00;
  msgTx.buf[7] = 0x00;
  can1.write(msgTx); 
}

void canSendSpeedMPH(int speedMPH){
  int speedKPH = (int)((float)speedMPH * 1.60934f);
  
  CAN_message_t msgTx;
  msgTx.len = 3; // number of bytes in request
  msgTx.id = 0x18FEF1FF;
  msgTx.buf[0] = 0x00;
  msgTx.buf[1] = 0x00;
  msgTx.buf[2] = (speedKPH & 0x00FF);         // Speed LSB
  msgTx.buf[3] = ((speedKPH & 0xFF00) >> 8);  // Speed MSB
  msgTx.buf[4] = 0x00;
  msgTx.buf[5] = 0x00; 
  msgTx.buf[6] = 0x00;
  msgTx.buf[7] = 0x00;
  can1.write(msgTx); 
}

void canReceive(const CAN_message_t &msg){
//  Serial.print("MB: ");
//  Serial.print(msg.mb);
//  Serial.print("  ID: 0x");
//  Serial.print(msg.id, HEX);
//  Serial.print("  EXT: ");
//  Serial.print(msg.flags.extended);
//  Serial.print("  LEN: ");
//  Serial.print(msg.len);
//  Serial.print(" DATA: ");
//  for (uint8_t i = 0; i < 8; i++) {
//    Serial.print(msg.buf[i], HEX);
//    Serial.print(" ");
//  }
//  Serial.println("");

  // Handle mode 41 messages
  if (msg.id == 0x7E8 && msg.buf[1] == 0x41) {
    switch (msg.buf[2]) {

      // RPM
      case 0x0c: {
        if (msg.buf[0] == 4) {
          values.rpm = ((msg.buf[3] * 256 + msg.buf[4]) / 4);
        }
      }
      break;

      // Coolant
      case 0x05: {
        if (msg.buf[0] == 3) {
          values.coolantTemp = (int)((((float)msg.buf[3] - 40.0f) * 1.8f) + 32.0f);
        }
      }
      break;

      // MAP
      case 0x0B: {
        if (msg.buf[0] == 3) {
          values.boostPressure = ((float)msg.buf[3] * 0.145038f) - values.barometricPressure;
        }
      }
      break;

      // Barometric pressure
      case 0x33: {
        if (msg.buf[0] == 3) {
          values.barometricPressure = (float)msg.buf[3] * 0.145038f;
        }
      }
      break;
      
    }
  }

  // Engine Fluid Level/Pressure 1
  if (msg.id == 0x18FEEF00) {
    // Oil pressure
    values.oilPressure = (float)msg.buf[3] * 4.0f * 0.145038f;
  }
}

void canInit() {
  can1.begin();
  can1.setBaudRate(500000);
  can1.enableFIFO();
  can1.enableFIFOInterrupt();
  can1.onReceive(canReceive);
}

#endif
