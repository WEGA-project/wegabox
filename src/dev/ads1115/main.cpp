#if c_ADS1115 == 1
  #include<ADS1115_WE.h> 
  #define I2C_ADDRESS 0x48
  ADS1115_WE adc = ADS1115_WE(I2C_ADDRESS);
  #define ADS1115_MiddleCount 10000 //6236ms за 10 тыс усреднений
   unsigned long ADS1115_old = millis();
unsigned long ADS1115_Repeat = 30000;
#endif
