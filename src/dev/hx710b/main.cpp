
#if c_HX710B == 1
  #include "HX710B.h"
  #define SCK_PIN 14
  #define SDI_PIN 13

  HX710B air_press(SCK_PIN, SDI_PIN);
  uint32_t time_update = 0;
#endif //c_HX710B