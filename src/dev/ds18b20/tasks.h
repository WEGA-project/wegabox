#if c_DS18B20 == 1
void TaskDS18B20(void *parameters)
{
  for (;;)
  {
    if (xSemaphoreX != NULL)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {

        //vTaskDelay(2000 / portTICK_PERIOD_MS);
        //sens18b20.begin();

        sens18b20.requestTemperatures();
        float ds0 = sens18b20.getTempCByIndex(0);
        if (ds0 != -127 and ds0 != 85)
        {
          RootTempRM.add(ds0);
          //RootTemp = RootTempMediana.filtered(ds0);
          //vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        else
          sens18b20.begin();

        RootTemp = RootTempRM.getMedian();
        xSemaphoreGive(xSemaphoreX);
      }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
#endif //c_DS18B20