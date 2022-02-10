#if c_LCD == 1
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
#include  <dev/lcd/func.h>
#endif // c_LCD