#ifndef DYPL07_H
#define DYPL07_H
#include <Arduino.h>
#include <Wire.h>
class DYPL07 {
  private:
    uint8_t _address;
    uint8_t _algorithm;
    // Moduri de selecție a algoritmului (0-5):
    // 0: Valoare în timp real;
    // 1-3: Filtrare valuri suprafață lichid (nivel 1-3), 1 cel mai mic, 3 cel mai mare;
    // 4: Mod de filtrare pași mici;
    // 5: Mod de sensibilitate ridicată.
    // Moduri implicite: UART automat (1), UART manual, PWM, RS485, IIC (0).

    uint16_t _distance;
    //aici salveaza distanta ca valoare numerica in cm

    uint8_t _signalLevel;
    // Nivelul semnalului (1-9, default: 5):
    // 1: ~50cm; 2: ~80cm; 3: ~110cm; 4: ~170cm; 5: ~200cm;
    // 6: ~210cm; 7: ~260cm; 8: ~280cm; 9: ~300cm.


    uint8_t _powerNoiseReductionLevel;
    // Nivelul de reducere a zgomotului alimentării (1-5):
    // 1: Baterii; 2: USB cu zgomot; 3: USB pe distanțe lungi;
    // 4: Surse comutabile; 5: Interferențe complexe (nerecomandat).

  public:
    DYPL07(uint8_t address8 = 0xE8);
    void writeNewAddress(uint8_t newAddress = 0xE8);
    // Adresa dispozitivului slave pe 8 biți (default: 0xE8):
    // Poate fi setată la oricare dintre cele 20 de adrese:
    // 0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE, 0xE0,
    // 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE, 0xF8, 0xFA, 0xFC, 0xFE.

    void setAlgorithm(uint8_t algorithm = 0);
    void setSignalLevel(uint8_t signalLevel = 5);
    void setPowerNoiseReductionLevel(uint8_t level = 1);
    void setAddress(uint8_t address = 0xE8);
    void sendMeasureRequest();
    void sendReceiveRequest();
    uint8_t getAddress();
    uint8_t getSignalLevel();
    uint8_t getAlgorithm();
    uint16_t getDistance();
};
#endif
