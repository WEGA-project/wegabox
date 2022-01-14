// Измерение термистора
#if c_NTC == 1
void TaskNTC(void *parameters)
{
  for (;;)
  {

    // if (xSemaphoreTake(xSemaphore, (TickType_t)1) == pdTRUE)
    // {

      unsigned long NTC0 = 0;
      for (long i = 0; i < NTC_MiddleCount and OtaStart != true and ECwork != true; i++)
      {
        NTC0 = NTC0 + adc1_get_raw(NTC_port);
      }
      NTCRM.add(float(NTC0) / NTC_MiddleCount);
      NTC = NTCRM.getAverage();
      //xSemaphoreGive(xSemaphore);
      vTaskDelay(10000 / portTICK_PERIOD_MS);
   // }    
    
  }
}
#endif // c_NTC