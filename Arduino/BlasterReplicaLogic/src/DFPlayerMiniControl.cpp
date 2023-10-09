#include "DFPlayerMiniControl.h"

DFPlayerMiniControl::DFPlayerMiniControl(uint8_t rt, uint8_t tx)
{
  soft_serial = new SoftwareSerial(rt, tx);

  soft_serial->begin(9600);
  delay(500);
  moduleInit();
  //volume(0x10);
}

void DFPlayerMiniControl::execute_CMD(uint8_t Command, uint8_t Data1, uint8_t Data2)
{
  word Checksum = -( Version_Byte + Command_Length + Command + Acknowledge + Data1 + Data2 );
  uint8_t command_line[10] = {
    Start_Byte,
    Version_Byte,
    Command_Length,
    Command,
    Acknowledge,
    Data1,
    Data2,
    highByte(Checksum),
    lowByte(Checksum),
    End_Byte
  };

  for(uint8_t k = 0; k < 10; k++)
  {
    soft_serial->write(command_line[k]);
  }
}

void DFPlayerMiniControl::moduleInit()
{
  execute_CMD(0x0C, 0, 0); delay(1000);
}

void DFPlayerMiniControl::volume(uint8_t volume)
{
  execute_CMD(0x06, 0, volume);
}

void DFPlayerMiniControl::playMp3(uint16_t track)
{
  execute_CMD(0x12, 0, track);
}

void DFPlayerMiniControl::advert(uint16_t track)
{
  execute_CMD(0x13, 0, track);
}

bool DFPlayerMiniControl::trackEnded() {
  return dataReceived();
}

bool DFPlayerMiniControl::dataReceived() {
  if(soft_serial->available() < 10) {
    return false;
  }
  for(uint8_t i = 0; i < 10; i++) {
    soft_serial->read();
  }
  return true;
}