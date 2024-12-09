#include <Arduino.h>
#include <Wire.h>
#include <XGZP6897D.h>

#define RANGE 2
//1-50 cm, 2-150 cm, 3-250cm, 4-350cm

uint8_t recv_buf[10] = { 0 };
uint16_t distance;

#define K 128  // see table above for the correct value for your sensor, according to the sensitivity.

// create the sensor object, passing the correct K value.
XGZP6897D mysensor(K);
int16_t temperature;
int32_t pressure;


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(38400);
  Wire.begin();
  setAlgorithmPattern(0x01);
  setSignalLevel(0x0);
  while (!mysensor.begin())  // initialize and check the device
  {
    Serial.println("Device not responding.");
    delay(500);
  }
  analogWrite(11,255);
}

// void setConeAngle() {
//   Wire.beginTransmission(0x74);
//   //Wire.write(0x02);  //doar cere valoarea stocata !!! nu are sens, do not touch
//   byte b1 = 0x07;
//   byte b2;
//   switch (CONE_ANGLE) {
//     case 1:
//       b2 = (0x01);
//       break;
//     case 2:
//       b2 = (0x02);
//       break;
//     case 3:
//       b2 = (0x03);
//       break;
//     case 4:
//       b2 = (0x04);
//       break;
//     default:
//       break;
//   }
//   Wire.write(b1);
//   Wire.write(b2);
//   Wire.endTransmission();
// }
void setAlgorithmPattern(byte b) {
  Wire.beginTransmission(0x74);
  Wire.write(0x09);
  Wire.write(b);
  Wire.endTransmission();
}
void setSignalLevel(byte b) {
  Wire.beginTransmission(0x74);
  Wire.write(0x07);
  Wire.write(b);
  Wire.endTransmission();
}
void sendMeasureRequest() {
  Wire.beginTransmission(0x74);
  Wire.write(0x10);
  Wire.write(0x14);
  Wire.endTransmission();
}
void sendReceiveRequest() {
  Wire.beginTransmission(0x74);
  Wire.write(0x02);  //cere valoare stocata
  int len = 0;
  //0x02 cere valoarea stocata in senzor, in functie de ce comanda a fost data
  //poate fi microsecunde sau mm
  Wire.endTransmission(0);
  Wire.requestFrom(0x74, 2);
  while (Wire.available()) {
    recv_buf[len++] = Wire.read();
  }
  if (len >= 2) {
    len = 0;
    distance = recv_buf[0] << 8 | recv_buf[1];
  }
}
void loop() {
  // static uint32_t millisSend = 0;
  static uint32_t millisReceive = 0;
  // if (millis() - millisSend > 50) {
  //   sendMeasureRequest();
  //   millisSend = millis();
  // } else {
  //   if (millis() - millisReceive > 10UL) {
  //     sendReceiveRequest();
  //     if (distance < 1500) {
  //       millisReceive = millis();
  //       Serial.println(distance);
  //     }
  //     else{
  //       delay(1);
  //     }
  //   }
  // }
  //sendMeasureRequest();
  //delay(200);
  sendReceiveRequest();
  delay(300);
  mysensor.readRawSensor(temperature, pressure);
  delay(100);
  temperature = temperature / 256;
  pressure = pressure / K;
  //if (distance != 65535 and distance != 65533) 
  {
    Serial.print(millis());
    Serial.print(',');
    Serial.print(distance);
    Serial.print(',');
    Serial.println(pressure);
  }
  delay(100);
}
