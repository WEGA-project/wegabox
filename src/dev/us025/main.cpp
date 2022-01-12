#if c_US025 == 1
  #define US_ECHO 13
  #define US_TRIG 14
  #include <HCSR04.h>
  UltraSonicDistanceSensor distanceSensor(US_ECHO, US_TRIG);
  #define US_MiddleCount 6000

#endif // c_US025