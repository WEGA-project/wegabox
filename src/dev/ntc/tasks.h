// Измерение термистора
#if c_NTC == 1
void TaskNTC(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
//if (xSemaphoreX != NULL and millis() - NTC_old > NTC_Repeat)
    if (millis() - NTC_old > NTC_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)10) == pdTRUE)
      {
        syslog.log(LOG_INFO, fFTS((millis() - NTC_old), 0) + " NTC REPEAT");
        syslog.log(LOG_INFO, fFTS(millis() / 1000, 3) + " sec Start NTC");

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

        syslog.log(LOG_INFO, fFTS(millis() / 1000, 3) + " sec NTC " + fFTS(NTC, 3));
        syslog.log(LOG_INFO, fFTS(millis() / 1000, 3) + " sec End NTC");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
      }
    }

    delay (10);
  }
}
#endif // c_NTC