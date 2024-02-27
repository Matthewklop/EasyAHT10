#include <Arduino.h>
#include <Wire.h>


#define AHT10_ADDRESS 0x38  // Change this if your sensor has a different address

void setup() {
  Wire.begin();  // Join I2C bus
  Serial.begin(115200);
}

void loop() {
  uint8_t data[6];
  float temperature, humidity;

  // Start a measurement
  Wire.beginTransmission(AHT10_ADDRESS);
  Wire.write(0xAC); // Measurement command for AHT10
  Wire.write(0x33); // Command to trigger measurement
  Wire.write(0x00); // Additional data byte for AHT10 command, not used in measurement
  Wire.endTransmission();

  // Wait for the measurement to complete
  // Typical measurement time is up to 80ms
  delay(80); 

  // Request data
  Wire.requestFrom(AHT10_ADDRESS, 6); // Request 6 bytes from the AHT10
  for (int i = 0; i < 6; i++) {
    if (Wire.available()) data[i] = Wire.read();
  }

  // Process the data
  if ((data[0] & 0x80) == 0) { // Check if the measurement is done (status bit 7 should be low)
    humidity = ((data[1] << 12) | (data[2] << 4) | (data[3] >> 4)) * 0.000095;
    temperature = (((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]) * 0.000191 - 50;
  } else {
    Serial.println("Measurement not completed");
  }

  // Output results
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("C ");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
}
