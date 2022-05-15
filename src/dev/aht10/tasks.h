#if c_AHT10 == 1
void TaskAHT10(void *parameters)
{
  for (;;)
  { 
    if (OtaStart == true)
      vTaskDelete(NULL);
    //syslog_ng("AHT10 loop");  
    //vTaskDelay(1000);
    delay(100);
    unsigned long AHT10_LastTime = millis() - AHT10_old;
    //if (AHT10_LastTime > AHT10_Repeat)
    if (xSemaphoreX != NULL and AHT10_LastTime > AHT10_Repeat)
    { //syslog_ng("AHT10 Semaphore");
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        unsigned long AHT10_time = millis();
        syslog_ng("AHT10 Start " + fFTS(AHT10_LastTime - AHT10_Repeat, 0) + "ms");
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
        AHT10_time = millis() - AHT10_time;
        syslog_ng("AHT10 AirTemp:" + fFTS(AirTemp, 3));
        syslog_ng("AHT10 AirHum:" + fFTS(AirHum, 3));        
        syslog_ng("AHT10 " + fFTS(AHT10_time, 0) + "ms end.");
        AHT10_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}

#endif // c_AHT10