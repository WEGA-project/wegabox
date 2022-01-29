#if c_DualBMx == 1
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

#include <BMx280I2C.h>

BMx280I2C bmx280_1(0x76);
BMx280I2C bmx280_2(0x77);

unsigned long DualBMx_old = millis();
unsigned long DualBMx_Repeat = 10000;
RunningMedian DstRM = RunningMedian(10);

#endif //c_DualBMx