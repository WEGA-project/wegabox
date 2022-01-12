#if c_DS18B20 == 1
void TaskDS18B20(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
    {
      vTaskDelete(NULL);
    }
    else
    {
      //vTaskDelay(2000 / portTICK_PERIOD_MS);
      //sens18b20.begin();
    if ( xSemaphoreTake( xI2CSemaphore, ( TickType_t ) 5 ) == pdTRUE )      {  
      sens18b20.requestTemperatures();
      float ds0 = sens18b20.getTempCByIndex(0);
      if (ds0 != -127 and ds0 != 85)        {
        RootTempRM.add(ds0);
        //RootTemp = RootTempMediana.filtered(ds0);
        //vTaskDelay(2000 / portTICK_PERIOD_MS);
      } else sens18b20.begin();
      xSemaphoreGive(xI2CSemaphore);}
      RootTemp = RootTempRM.getMedian();
      
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}
#endif //c_DS18B20