#include "DYPL07.h"
DYPL07::DYPL07(uint8_t address8 = 0xE8) {
  setAddress(address8);
}
void DYPL07::setAddress(uint8_t address8 = 0xE8) {
  _address = (address8) >> 1;
}
void DYPL07::writeNewAddress(uint8_t newAddress = 0xE8) {
  if ((newAddress >> 7) & 1) {  //verifica sa fie adresa buna, pe 8 biti
    Wire.beginTransmission(_address);
    Wire.write(0x05);
    Wire.write(newAddress);
    Wire.endTransmission();
    setAddress(newAddress);
  }
}
void DYPL07::setAlgorithm(uint8_t algorithm = 0) {
  Wire.beginTransmission(_address);
  Wire.write(0x09);
  Wire.write(algorithm);
  Wire.endTransmission();
  _algorithm = algorithm;  //TODO: vazut cu Vio daca e necesar de facut si un check daca chiar a salvat si pe senzor valoarea
}
void DYPL07::setSignalLevel(uint8_t signalLevel = 5) {
  Wire.beginTransmission(_address);
  Wire.write(0x07);
  Wire.write(signalLevel);
  Wire.endTransmission();
  _signalLevel = signalLevel;  //TODO: vazut si aici daca e nevoie de check
}
void DYPL07::setPowerNoiseReductionLevel(uint8_t level = 1) {
  Wire.beginTransmission(_address);
  Wire.write(0x06);
  Wire.write(level);
  Wire.endTransmission();
  _powerNoiseReductionLevel = level;  //TODO: vazut si aici daca e nevoie de check
}
void DYPL07::sendMeasureRequest() {  //doar pe manual
  Wire.beginTransmission(_address);
  Wire.write(0x10);
  Wire.write(0x14);
  Wire.endTransmission();
}
void DYPL07::sendReceiveRequest() {
  Wire.beginTransmission(_address);
  Wire.write(0x02);  //cere valoare stocata
  Wire.endTransmission();
  //0x02 cere valoarea stocata in senzor, in functie de ce comanda a fost data, timpul de ping pong sau distanta
  //poate fi microsecunde sau mm

  Wire.requestFrom(0x74, 2);
  uint16_t b1 = Wire.read();  //byte 1
  uint16_t b2 = Wire.read();  //byte 2
  _rawDistance = (b1 << 8) | b2;
  timeMeasure = millis();
}

uint16_t DYPL07::getRawDistance() {
  return _rawDistance;
}
int16_t DYPL07::getDistance() {
  /*
  Return values:
  -4: Old value (>2 sec, TODO: un timp okay)
  -3: Not found (0xFFFD)
  -2: Interference (0xFFFE)
  -1: Ranging not completed (0xFFFF)
  */
  if (millis() - timeMeasure > 2*(1000UL)) {  // 2 seconds timeout
    _distance = -4;
  } else if (_rawDistance == 0xFFFD) { //Not found
    _distance = -3;
  } else if (_rawDistance == 0xFFFE) { //Interference
    _distance = -2;
  } else if (_rawDistance == 0xFFFF) { //Ranging not completed
    _distance = -1;
  } else{
    _distance = int16_t(_rawDistance);
  }

  return _distance;
}
uint8_t DYPL07::getAlgorithm() {
  return _algorithm;
}
