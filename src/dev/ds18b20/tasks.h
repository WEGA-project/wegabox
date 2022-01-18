#if c_DS18B20 == 1
void TaskDS18B20(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);

    unsigned long DS18B20_LastTime = millis() - DS18B20_old;

    if (xSemaphoreX != NULL and DS18B20_LastTime > DS18B20_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        unsigned long DS18B20_time = millis();
        syslog_ng("DS18B20 Start " + fFTS(DS18B20_LastTime - DS18B20_Repeat, 0) + "ms");
        //vTaskDelay(2000 / portTICK_PERIOD_MS);
        //sens18b20.begin();

        // if (sens18b20.getDeviceCount() > 0)
        // {
          //syslog_ng("DS18B20 status:" +fFTS(sens18b20.getDeviceCount()  );
          sens18b20.begin();
          while (sens18b20.getCheckForConversion() == false);

          sens18b20.requestTemperatures();
          float ds0 = sens18b20.getTempCByIndex(0);
          if (ds0 != -127 and ds0 != 85)
          {
            RootTempRM.add(ds0);
            //RootTemp = RootTempMediana.filtered(ds0);
            //vTaskDelay(2000 / portTICK_PERIOD_MS);
          }
          // else
          //   sens18b20.begin();

          RootTemp = RootTempRM.getMedian();
        // }
        
        xSemaphoreGive(xSemaphoreX);
        DS18B20_time = millis() - DS18B20_time;
        syslog_ng("DS18B20 " + fFTS(RootTemp, 3));
        syslog_ng("DS18B20 " + fFTS(DS18B20_time, 0) + "ms end.");
        DS18B20_old = millis();

        vTaskDelay(1000 / portTICK_PERIOD_MS);
      }
    }
    vTaskDelay(1);
  }
}
#endif //c_DS18B20