#if c_US025 == 1
  #define US_ECHO 13
  #define US_TRIG 14
  #include <HCSR04.h>
  UltraSonicDistanceSensor distanceSensor(US_ECHO, US_TRIG);
  #define US_MiddleCount 6000

unsigned long US025_old = millis();
unsigned long US025_Repeat = 5000;
#endif // c_US025