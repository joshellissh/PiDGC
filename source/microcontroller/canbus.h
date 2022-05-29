#ifndef CANBUS_H
#define CANBUS_H

#include <FlexCAN_T4.h>

class Canbus {
private:
  FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> canBus;

public:
  void init();

  // Sends a CAN request
  void send(uint8_t activeByte);
};

#endif
