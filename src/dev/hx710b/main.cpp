
#if c_HX710B == 1
  #include "HX710B.h"
  #define SCK_PIN 13
  #define SDI_PIN 14

  HX710B HX710B_Press(SCK_PIN, SDI_PIN);
  uint32_t time_update = 0;
unsigned long HX710B_old = millis();
unsigned long HX710B_Repeat = 10000;

float HX710B_X1=5144440;  // АЦП первой точки
float HX710B_H1=0;        // Уровень первой точки

float HX710B_X2=6001620; // АЦП второй точки
float HX710B_H2=15;      // Уровень второй точки

float HX710B_a=(HX710B_X1*HX710B_H2-HX710B_H1*HX710B_X2)/(HX710B_X1-HX710B_X2);
float HX710B_b=(HX710B_H1-HX710B_H2)/(HX710B_X1-HX710B_X2);

RunningMedian HX710B_DistRM = RunningMedian(30);

#endif //c_HX710B