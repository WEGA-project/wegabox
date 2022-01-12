#if c_NTC == 1
 #define NTC_port ADC1_CHANNEL_4 // gpio32
 #define NTC_MiddleCount 900000
#endif

#if c_EC == 1
  #define EC_DigitalPort1 18
  #define EC_DigitalPort2 19
  #define EC_AnalogPort ADC1_CHANNEL_5 // gpio33
  #define EC_MiddleCount 500000  // 12000 в секунду
#endif