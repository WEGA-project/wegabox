// Измерение термистора
#if c_NTC == 1
void TaskNTC(void *parameters)
{
  for (;;)
  {if (OtaStart == true) vTaskDelete(NULL);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    if (xSemaphore != NULL)
    {
      // rtc_wdt_protect_on();
      // rtc_wdt_enable();
      // enableCore0WDT();
      // enableLoopWDT();
     
      if (xSemaphoreTake(xSemaphore, (TickType_t)25) == pdTRUE)
      {
        
        long ntct = millis();
        unsigned long NTC0 = 0;
        for (long i = 0; i < NTC_MiddleCount and OtaStart != true; i++)
        {
          NTC0 = NTC0 + adc1_get_raw(NTC_port);

          if (millis() - ntct > 1000)
          {
            vTaskDelay(10 / portTICK_PERIOD_MS);
            ntct = millis();
          }
        }
        NTCRM.add(float(NTC0)/NTC_MiddleCount);
        //NTC = float(NTC0) / NTC_MiddleCount;
        NTC = NTCRM.getMedian();
        xSemaphoreGive(xSemaphore);
      }

    }

    
  }
}
#endif // c_NTC