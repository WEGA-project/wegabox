#if c_PR == 1
 #define PR_AnalogPort ADC1_CHANNEL_7 // gpio35
 #define PR_MiddleCount 1000

unsigned long PR_old = millis();
unsigned long PR_Repeat = 10000;
#endif // c_PR