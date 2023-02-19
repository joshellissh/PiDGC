 #ifndef CANBUS_H
#define CANBUS_H

#include <FlexCAN_T4.h>
#include <math.h>
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

void canSendSpeedMPH(float speedMPH){
  float fspeedKPH = speedMPH * 1.609344f;
  byte bspeedKPH = (byte)floor(fspeedKPH);
  byte bspeedDecimalsKPH = (byte)round(fmod(fspeedKPH, 1) * 256.0);
  
  CAN_message_t msgTx;
  msgTx.len = 3; // number of bytes in request
  msgTx.id = 0x18FEF1FF;
  msgTx.flags.extended = 1;
  msgTx.buf[0] = 0x00; // byte 1
  msgTx.buf[1] = bspeedDecimalsKPH;
  msgTx.buf[2] = bspeedKPH;
  msgTx.buf[3] = 0x00;
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
          values.manifoldAbsolutePressure = ((float)msg.buf[3] * 0.145038f);
          values.boostPressure = values.manifoldAbsolutePressure - values.barometricPressure;
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

      // IAT
      case 0x0F: {
        if (msg.buf[0] == 3) {
          values.intakeAirTemp = (float)msg.buf[3] - 40;
        }
      }
      break;

      // Timing advance
      case 0x0E: {
        if (msg.buf[0] == 3) {
          values.timingAdvance = ((float)msg.buf[3] / 2.0f) - 64.0f;
        }
      }
      break;

      // Throttle position (relative)
      case 0x45: {
        if (msg.buf[0] == 3) {
          values.throttlePosition = (float)msg.buf[3] * 0.39215;
        }
      }
      break;

      // Foot Pedal Position (Relative)
      case 0x5A: {
        if (msg.buf[0] == 3) {
          values.footPedalPosition = (float)msg.buf[3] * 0.39215;
        }
      }
      break;

      // Oxygen Sensor 1 Air-Fuel Equivalence Ratio
      case 0x34: {
        // if (msg.buf[0] == 6) {
          values.oxySensor1AFER = 0.000030517578125f * ((256.0f * (float)msg.buf[3]) + (float)msg.buf[4]);
        // }
      }
      break;

      // Short Term Fuel Trim - Bank 1
      case 0x06: {
        if (msg.buf[0] == 3) {
          values.shortTermFuelTrimB1 = (0.78125f * (float)msg.buf[3]) - 100.0f;
        }
      }
      break;

      // Long Term Fuel Trim - Bank 1
      case 0x07: {
        if (msg.buf[0] == 3) {
          values.longTermFuelTrimB1 = (0.78125f * (float)msg.buf[3]) - 100.0f;
        }
      }
      break;

      // Throttle Inlet Pressure
      case 0x70: {
        // if (msg.buf[0] == 12) {
          values.throttleInletPressure = ((256.0f * (float)msg.buf[6]) + (float)msg.buf[7]) / 32.0f;
        // }
      }
      break;

      // Fuel Rail Pressure
      case 0x23: {
        if (msg.buf[0] == 4) {
          values.fuelRailPressure = 10.0f * ((256.0f * (float)msg.buf[3]) + (float)msg.buf[4]);
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
