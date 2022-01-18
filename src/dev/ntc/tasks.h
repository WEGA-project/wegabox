// Измерение термистора
#if c_NTC == 1
void TaskNTC(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);

      unsigned long NTC_LastTime=millis() - NTC_old;

    if (xSemaphoreX != NULL and NTC_LastTime > NTC_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        syslog_ng("NTC Start");
        syslog_ng("NTC Last time old " + fFTS(NTC_LastTime - NTC_Repeat,0));

        rtc_wdt_protect_off();
        rtc_wdt_disable();
        disableCore0WDT();
        disableLoopWDT();

        //unsigned long NTC0 = 0;
        float NTC0 = 0;
        for (long i = 0; i < NTC_MiddleCount and OtaStart != true ; i++)
        {
          NTC0 = NTC0 + adc1_get_raw(NTC_port);
        }

        rtc_wdt_protect_on();
        rtc_wdt_enable();
        enableCore0WDT();
        enableLoopWDT();

        NTCRM.add(float(NTC0) / NTC_MiddleCount);
        NTC = NTCRM.getAverage();
        xSemaphoreGive(xSemaphoreX);
        NTC_old = millis();

        syslog_ng("NTC End");
        //vTaskDelay(5000 / portTICK_PERIOD_MS);
      }
    }

            vTaskDelay(1);
  }
}
#endif // c_NTC