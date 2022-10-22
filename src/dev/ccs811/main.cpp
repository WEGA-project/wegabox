#if c_CCS811 == 1
uint8_t CCS811addr = 0x5b;

#include "ccs811.h" // CCS811 library
#include <dev/ccs811/CCS811_FW_App_v2-0-0.h>
CCS811 ccs811;
unsigned long CCS811_old = millis();
unsigned long CCS811_Repeat = 10000;

RunningMedian CCS811_eCO2RM = RunningMedian(10);
RunningMedian CCS811_tVOCRM = RunningMedian(10);
#endif