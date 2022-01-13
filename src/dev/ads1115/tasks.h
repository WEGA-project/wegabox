#if c_ADS1115 == 1
void TaskADS1115(void *parameters)
{
  for (;;)
  {

    if (xSemaphore != NULL)
    {
      if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE)
      {
        //vTaskDelay(2000 / portTICK_PERIOD_MS);
        pHraw = adc.getRawResult();

        long cont = 0;
        double sensorValue = 0;
        while (cont < ADS1115_MiddleCount and OtaStart != true)
        {
          cont++;
          sensorValue = adc.getResult_mV() + sensorValue;
        }

        PhRM.add(sensorValue / cont);
        pHmV = PhRM.getMedian();

        xSemaphoreGive(xSemaphore);
      }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
#endif // c_ADS1115