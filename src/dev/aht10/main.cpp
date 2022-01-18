#if c_AHT10 == 1
  #include <AHT10.h>
  uint8_t readStatus = 0;
  AHT10 myAHT10(AHT10_ADDRESS_0X38);
  String st_AHT10;

unsigned long AHT10_old = millis();
unsigned long AHT10_Repeat = 10000;
#endif