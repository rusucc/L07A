#include <Arduino.h>
#include <Wire.h>
#include "DYPL07.h"

DYPL07 sonarBazin;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(38400);
  Wire.begin();
  sonarBazin.setAddress(0xE8);
  // Adresa dispozitivului slave pe 8 biți (default: 0xE8/0x74 pe 7 biti):
  // Poate fi setată la oricare dintre cele 20 de adrese:
  // 0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE, 0xE0,
  // 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE, 0xF8, 0xFA, 0xFC, 0xFE.

  sonarBazin.setAlgorithm(0x00);
  // Moduri de selecție a algoritmului (0-5):
  // Cu request manual:
  // 0: Valoare în timp real;

  // Cu request automat:
  // 1-3: Filtrare valuri suprafață lichid (nivel 1-3), 1 cel mai mic, 3 cel mai mare;
  // 4: Mod de filtrare pași mici;
  // 5: Mod de sensibilitate ridicată.
  // Moduri implicite: UART automat (1), UART manual, PWM, RS485, IIC (0).

  sonarBazin.setSignalLevel(0x01);
  // Nivelul semnalului (1-9, default: 5):
  // 1: ~50cm; 2: ~80cm; 3: ~110cm; 4: ~170cm; 5: ~200cm;
  // 6: ~210cm; 7: ~260cm; 8: ~280cm; 9: ~300cm.

  sonarBazin.setPowerNoiseReductionLevel(0x03);
  // Nivelul de reducere a zgomotului alimentării (1-5):
  // 1: Baterii; 2: USB cu zgomot; 3: USB pe distanțe lungi;
  // 4: Surse comutabile; 5: Interferențe complexe (nerecomandat).
}

void loop() {
  if (sonarBazin.getAlgorithm() == 0x00) {
    sonarBazin.sendMeasureRequest();
    delay(100);
  }  //doar daca e pe algoritm 0 trebuie dat request de masurare manual, in rest senzorul calculeaza singur distanta si o trimite cand primeste request;
  delay(250);
  Serial.println(sonarBazin.getDistance());
}
