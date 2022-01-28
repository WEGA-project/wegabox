#if c_EC == 1

  #define EC_DigitalPort1 18
  #define EC_DigitalPort2 19
  #define EC_AnalogPort ADC1_CHANNEL_5 // gpio33
  #define EC_MiddleCount 100000  // 12000 в секунду

unsigned long EC_old = millis();
unsigned long EC_Repeat=20000;

RunningMedian ApRM = RunningMedian(10);
RunningMedian AnRM = RunningMedian(10);
#endif