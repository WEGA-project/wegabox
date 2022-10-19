// Измерение термистора
#if c_NTC == 1
void TaskNTC(void *parameters)
{

  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(1000);
    //syslog_ng("NTC loop");

    unsigned long NTC_LastTime = millis() - NTC_old;

    if (xSemaphoreX != NULL and NTC_LastTime > NTC_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        vTaskDelay(2000);
        unsigned long NTC_time = millis();
        syslog_ng("NTC Start " + fFTS(NTC_LastTime - NTC_Repeat, 0) + "ms");

        // rtc_wdt_protect_off();
        // rtc_wdt_disable();
        // disableCore0WDT();
        // disableLoopWDT();

        //unsigned long NTC0 = 0;
        float NTC0 = 0;
        for (long i = 0; i < NTC_MiddleCount and OtaStart != true; i++)
        {
          NTC0 = NTC0 + adc1_get_raw(NTC_port);
          vTaskDelay(1);
        }

        // rtc_wdt_protect_on();
        // rtc_wdt_enable();
        // enableCore0WDT();
        // enableLoopWDT();

        NTCRM.add(float(NTC0) / NTC_MiddleCount);
        NTC = NTCRM.getAverage();

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