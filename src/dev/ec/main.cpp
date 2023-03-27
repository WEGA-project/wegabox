#if c_EC == 1
#include <dev/old_adc/wega-adc.h>
static _lock_t sar_adc1_lock;
#define SAR_ADC1_LOCK_ACQUIRE() _lock_acquire(&sar_adc1_lock)
#define SAR_ADC1_LOCK_RELEASE() _lock_release(&sar_adc1_lock)

  #define EC_DigitalPort1 18
  #define EC_DigitalPort2 19
  #define EC_AnalogPort ADC1_CHANNEL_5 // gpio33
  #define EC_MiddleCount 100000  // 12000 в секунду

unsigned long EC_old = millis();
unsigned long EC_Repeat=20000;

RunningMedian ApRM = RunningMedian(10);
RunningMedian AnRM = RunningMedian(10);
#endif