#if c_VL6180X == 1

#include <Wire.h>
#include <VL6180X.h>

VL6180X s_vl6180X;

    unsigned long VL6180X_Count = 254; //усреднений
    unsigned long VL6180X_old = millis();
    unsigned long VL6180X_Repeat = 5000;

    RunningMedian VL6180X_RangeRM = RunningMedian(200);



#endif //c_VL6180X
