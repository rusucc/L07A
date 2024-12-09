#include <Arduino.h>
#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  Wire.begin();
  delay(100);
  Serial.println("Changing I2C");
  changeI2CAddressL07((0xE8)>>1,(0xD0)>>0); 
  /*
  Arduino stie sa se adreseze doar pe 7 biti, dar adresa trebuie data in 8 biti !!!
  De exemplu, in datasheet urmatoarele valori sunt valide pentru I2C
  -0xD0,D2,D4,D6,D8,DA,DC,DE
  -0xE0,E2,E4,E6,E8,EA,EC,EE
  -0xF8,FA,FC,FE
  Toate sunt adrese pe 8 biti, pt D0 in decimal vine 208
  Ca sa ne adresam la senzorul asta, trebuie shiftat la dreapta 1 bit, de unde iese valoarea 104, sau 0x68 in hex
  Dar ca sa scriem o valoare noua, trebuie sa fie pe 8 biti, deci scriem 0xD0, chiar daca ne adresam cu 0x68
  
  https://docs.arduino.cc/language-reference/en/functions/communication/wire/
  
  */
  Serial.println("Changed I2C");
  while(!Serial.available()){
    delay(10);
  }
}

void changeI2CAddressL07(uint8_t initialAddress, uint8_t finalAddress){
  Serial.println(initialAddress,HEX);
  Serial.println(finalAddress,HEX);
  Wire.beginTransmission(initialAddress);
  Wire.write(0x05);
  Wire.write(finalAddress);
  Wire.endTransmission();
}

void loop(){
  int nDevices = 0;

  Serial.println("Scanning...");

  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of
    // the Wire.endTransmission to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      ++nDevices;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  delay(5000); // Wait 5 seconds for next scan
}
