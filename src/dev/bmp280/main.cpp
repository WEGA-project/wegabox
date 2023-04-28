#if c_BMP280 == 1

#include <BMx280I2C.h>
uint8_t BMx280addr;

unsigned long BMx280_old = millis();
unsigned long BMx280_Repeat = 5000;

RunningMedian BMx280_AirTempRM = RunningMedian(30);
RunningMedian BMx280_AirHumRM = RunningMedian(30);
RunningMedian BMx280_AirPressRM = RunningMedian(30);

float BMx280_Temp, BMx280_Hum, BMx280_Press;
#endif //c_BMP280