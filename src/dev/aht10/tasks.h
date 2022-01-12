#if c_AHT10 == 1
void TaskAHT10(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    else
    {
      if (xSemaphoreTake(xI2CSemaphore, (TickType_t)5) == pdTRUE)
      {
        readStatus = myAHT10.readRawData();
        if (readStatus != AHT10_ERROR)
        {
          float AirTemp0 = myAHT10.readTemperature();
          float AirHum0 = myAHT10.readHumidity();
          if (AirTemp0 != 255 and AirHum0 != 255 and AirTemp0 != -50)
          {
            
            AirTempRM.add(AirTemp0);
            AirTemp = AirTempRM.getMedian();
            
            AirHumRM.add(AirHum0);
            AirHum = AirHumRM.getMedian();
          }
        }
        else
        {
          myAHT10.softReset();
          delay(100);
          myAHT10.begin();
          myAHT10.setNormalMode();
          myAHT10.setCycleMode();
        }
        xSemaphoreGive(xI2CSemaphore); }
      
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}
#endif // c_AHT10