#if c_ADS1115 == 1
  #include<ADS1115_WE.h> 

  uint8_t ADS1115addr=0x48;
  ADS1115_WE adc = ADS1115_WE(ADS1115addr);
  #define ADS1115_MiddleCount 10000 //6236ms за 10 тыс усреднений
   unsigned long ADS1115_old = millis();
unsigned long ADS1115_Repeat = 30000;
#endif
