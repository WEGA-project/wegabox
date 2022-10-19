#if c_US025 == 1
#define US_ECHO 13
#define US_TRIG 14
#include <HCSR04.h>
UltraSonicDistanceSensor distanceSensor(US_ECHO, US_TRIG);

#define US_MiddleCount 6000
RunningMedian DstRM = RunningMedian(20);

unsigned long US025_old = millis();
unsigned long US025_Repeat = 10000;
#endif // c_US025