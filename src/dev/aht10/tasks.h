#if c_AHT10 == 1
void TaskAHT10(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    unsigned long AHT10_LastTime = millis() - AHT10_old;
    if (xSemaphoreX != NULL and AHT10_LastTime > AHT10_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        syslog_ng("AHT10 Start");
        syslog_ng("AHT10 Last time old " + fFTS(AHT10_LastTime - AHT10_Repeat, 0));
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
        xSemaphoreGive(xSemaphoreX);
        AHT10_old = millis();
        syslog_ng("AHT10 End");
      }
    }
        vTaskDelay(10);
  }
}

#endif // c_AHT10