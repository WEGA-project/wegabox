#if c_NTC == 1

RunningMedian NTCRM = RunningMedian(20);
 #define NTC_port ADC1_CHANNEL_4 // gpio32
 #define NTC_MiddleCount 10000
unsigned long NTC_old = millis();
unsigned long NTC_Repeat = 5000;
#endif