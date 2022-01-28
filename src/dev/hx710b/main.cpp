
#if c_HX710B == 1
  #include "HX710B.h"
  #define SCK_PIN 13
  #define SDI_PIN 14

  HX710B air_press(SCK_PIN, SDI_PIN);
  uint32_t time_update = 0;

  unsigned long HX710B_old = millis();
unsigned long HX710B_Repeat = 10000;
#endif //c_HX710B