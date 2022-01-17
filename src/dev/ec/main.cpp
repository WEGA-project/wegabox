#if c_EC == 1

  #define EC_DigitalPort1 18
  #define EC_DigitalPort2 19
  #define EC_AnalogPort ADC1_CHANNEL_5 // gpio33
  #define EC_MiddleCount 500000  // 12000 в секунду
  unsigned long t_EC=0;
//,t_Dist,t_NTC,t_pH;
float f_EC=0;
unsigned long EC_old = millis();
unsigned long EC_Repeat=90000;
#endif