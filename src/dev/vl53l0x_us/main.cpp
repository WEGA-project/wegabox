#if c_VL53L0X_us == 1


#include <Wire.h>
#include <VL53L0X.h>

VL53L0X s_VL53L0X;
// #define LONG_RANGE
// //#define HIGH_ACCURACY
// #define HIGH_SPEED

    unsigned long VL53L0X_Count = 254; //усреднений
    unsigned long VL53L0X_old = millis();
    unsigned long VL53L0X_Repeat = 20000;

    RunningMedian VL53L0X_RangeRM = RunningMedian(5);



#endif //c_VL53L0X_us
