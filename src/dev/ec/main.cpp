#if c_EC == 1

  #define EC_DigitalPort1 18
  #define EC_DigitalPort2 19
  #define EC_AnalogPort GPIO_NUM_33 // gpio33
  #define EC_MiddleCount 60000 // 60000 за 1 сек

unsigned long EC_old = millis();
unsigned long EC_Repeat=5000;

RunningMedian ApRM = RunningMedian(5);
RunningMedian AnRM = RunningMedian(5);
#endif