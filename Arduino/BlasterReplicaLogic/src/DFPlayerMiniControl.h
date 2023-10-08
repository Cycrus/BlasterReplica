#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "SoftwareSerial.h"

#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00

class DFPlayerMiniControl
{
  public:
    DFPlayerMiniControl(uint8_t rt, uint8_t tx);

    void execute_CMD(byte Command, byte Data1, byte Data2);

    void moduleInit();

    void volume(uint8_t volume);

    void playMp3(uint16_t track);

    void advert(uint16_t track);

    bool trackEnded();

  private:
    SoftwareSerial* soft_serial;

    bool dataReceived();
};