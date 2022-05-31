#include "canbus.h"
#include "globals.h"

// Interrupt handler for Mailbox 1
void receive(const CAN_message_t &msg){
  Serial.println(" ");
  Serial.print("MB: ");
  Serial.print(msg.mb);
  Serial.print("  ID: 0x");
  Serial.print(msg.id, HEX);
  Serial.print("  EXT: ");
  Serial.print(msg.flags.extended);
  Serial.print("  LEN: ");
  Serial.print(msg.len);
  Serial.print(" DATA: ");
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print(msg.buf[i], HEX);
    Serial.print(" ");
  }

  // Handle mode 41 messages
  if (msg.id == 0x7E8 && msg.buf[1] == 0x62 && msg.buf[2] == 0xf4) {
    switch (msg.buf[3]) {

      // RPM
      case 0x0c: {
        if (msg.len == 4)
          values.rpm = ((msg.buf[4] * 256 + msg.buf[5]) / 4);
      }
      break;

      // Coolant
      case 0x05: {
        if (msg.len == 3)
          values.coolantTemp = (int)((((float)msg.buf[3] - 40.0f) * 1.8f) + 32.0f);
      }
      break;

      // MAP
      case 0x0B: {
        if (msg.len == 3)
          values.boostPressure = ((float)msg.buf[3] * 0.145038f) - values.barometricPressure;
      }
      break;

      // Barometric pressure
      case 0x33: {
        if (msg.len == 3)
          values.barometricPressure = (float)msg.buf[3] * 0.145038f;
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

void Canbus::init() {
  canBus.begin();
  canBus.setBaudRate(500000);
  // Default to rejecting all messages
  canBus.setMBFilter(REJECT_ALL);
  // Accept Frame ID 0x7E8 in Mailbox 1
  canBus.setMBFilter(MB1, 0x7E8, 0x18FEEF00);
  // Enable enhanced filtering for MB1
  canBus.enhanceFilter(MB1);
  // Set interrupt on MB1
  canBus.enableMBInterrupt(MB1);
  // Add callback for canBus
  canBus.onReceive(receive);
}

void Canbus::events() {
  canBus.events();
}

void Canbus::send(uint8_t activeByte){
  CAN_message_t msgTx;
  msgTx.len = 3; // number of bytes in request
  msgTx.id = 0x7E0; // Request Header
  //  msgTx.flags.extended = 0; // 11 bit header, not 29 bit
  //  msgTx.flags.remote = 0;
  msgTx.buf[0] = 0x02; // Number of bytes in request
  msgTx.buf[1] = 0x01; // Mode of request (0x01 = OBD2)
  msgTx.buf[2] = activeByte;
  msgTx.buf[4] = 0x00;
  msgTx.buf[5] = 0x00; 
  msgTx.buf[6] = 0x00;
  msgTx.buf[7] = 0x00;
  canBus.write(msgTx); 
}
