#include <Arduino.h>
#include <Wire.h>

//enable esp32 logging

#include "esp32-hal-log.h"

int requestX(byte address)
{
  Wire.beginTransmission(0xC);
  Wire.write(0x0A); 
  Wire.write(0x01); 
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(0xC);
  Wire.write(address); 
  Wire.endTransmission();
  delay(10);

  Wire.requestFrom(0xC, 2);
  //delay(100);

  byte x[2];
  int i = 0;
  while (!Wire.available());
  delay(5);
  while (Wire.available()) 
  {
    if(i == 2)
    {
      break;
    }
    x[i] = Wire.read();
    //log_d("Readed x[%d] = %d", i, x[i]);
    i++;

  }

  int out=0;
  out = x[1];
  out |= x[0] <<8;
  //convert out in 2s complement to decimal
  if(out > 32767)
  {
    out = out - 65536;
  }

  
  return out;

}

void setup() {
  // put your setup code here, to run once:
  //start serial
  Serial.begin(115200);
  Serial.println("Hello World!");

  //start i2c
  Wire.begin();

  //i2c scanner
  Serial.println("Scanning I2C bus...");
  byte count = 0;
  for (byte i = 8; i < 120; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found address: ");
      Serial.print(i, DEC);
      Serial.print(" (0x");
      Serial.print(i, HEX);
      Serial.println(")");
      count++;
      delay(100);  // maybe unneeded?
    }
  }
  if (count == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }


  
}



void loop() {
  // put your main code here, to run repeatedly:
  
  log_d("X: %d, Y: %d, Z: %d", requestX(0x3), requestX(0x5), requestX(0x7));

  //delay(1000);
}