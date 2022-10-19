#if c_BMP280 == 1
// Arduino - BMP280 / BME280
// 3.3V ---- VCC
// GND ----- GND
// SDA ----- SDA
// SCL ----- SCL
// some BMP280/BME280 modules break out the CSB and SDO pins as well: 
// 5V ------ CSB (enables the I2C interface)
// GND ----- SDO (I2C Address 0x76)
// 5V ------ SDO (I2C Address 0x77)
// other pins can be left unconnected.

//#include <Arduino.h>
//#include <Wire.h>
//#include <Adafruit_Sensor.h>
#include <BMx280I2C.h>

//#define I2C_ADDRESS 0x76

//create a BMx280I2C object using the I2C interface with I2C Address 0x76
BMx280I2C bmx280(0x76);

unsigned long BMP280_old = millis();
unsigned long BMP280_Repeat = 10000;

RunningMedian BMP280_AirTempRM = RunningMedian(30);
RunningMedian BMP280_AirHumRM = RunningMedian(30);
RunningMedian BMP280_AirPressRM = RunningMedian(30);

float BMP280_Temp, BMP280_Hum, BMP280_Press;
#endif //c_BMP280