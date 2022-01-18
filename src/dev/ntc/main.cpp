#if c_NTC == 1
 #define NTC_port ADC1_CHANNEL_4 // gpio32
 #define NTC_MiddleCount 90000
unsigned long NTC_old = millis();
unsigned long NTC_Repeat = 30000;
#endif