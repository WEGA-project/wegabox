#if c_NTC == 1 
void TaskNTC(void *parameters) {
  for (;;) {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(100);

    unsigned long NTC_LastTime = millis() - NTC_old;

    if (xSemaphoreX != NULL and NTC_LastTime > NTC_Repeat) {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE) {
        vTaskDelay(1000); // Нужно выждать, чтобы убрать электромагнитные наводки от ЕС
        unsigned long NTC_time = millis();
        syslog_ng("NTC Start " + fFTS(NTC_LastTime - NTC_Repeat, 0) + "ms");

        adc_power_acquire();
        SAR_ADC1_LOCK_ACQUIRE();
        vTaskDelay(300);

        // unsigned long NTC0 = 0;
        float NTC0 = 0;
        for (long i = 0; i < NTC_MiddleCount and OtaStart != true; i++) {

                    __wega_adcStart(NTC_port);
                    vTaskDelay(1);
          NTC0 = NTC0 + __wega_adcEnd(NTC_port);

          // NTC0 = NTC0 + adc1_get_raw(NTC_port);
          // vTaskDelay(1);
        }

        SAR_ADC1_LOCK_RELEASE();
        adc_power_release();

        NTCRM.add(float(NTC0) / NTC_MiddleCount);
        NTC = NTCRM.getAverage(10);

        NTC_time = millis() - NTC_time;
        syslog_ng("NTC:" + fFTS(NTC, 3));

        syslog_ng("NTC " + fFTS(NTC_time, 0) + "ms end.");
        NTC_old = millis();
        xSemaphoreGive(xSemaphoreX);
        syslog_ng("NTC Semaphore Give");
      }
    }
  }
}
#endif // c_NTC