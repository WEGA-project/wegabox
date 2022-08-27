#if c_VL53L0X_us == 1

#define US_SDA 13 // SDA
#define US_SCL 14 // SCL

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X s_VL53L0X;

// unsigned long VL53L0X_Count = 254; //усреднений
unsigned long VL53L0X_old = millis();
unsigned long VL53L0X_Repeat = 5000;

RunningMedian VL53L0X_RangeRM = RunningMedian(240);
long VL53L0X_err = 0;

#endif // c_VL53L0X_us
