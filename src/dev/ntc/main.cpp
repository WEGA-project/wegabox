#if c_NTC == 1
 #define NTC_port ADC1_CHANNEL_4 // gpio32
 #define NTC_MiddleCount 900000
 unsigned long NTC_old = 0;
unsigned long NTC_Repeat = 60000;
#endif