#if c_DS18B20 == 1
void TaskDS18B20(void *parameters)
{
  vTaskDelay(10000);
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    //vTaskDelay(1000);
    delay(100);

    unsigned long DS18B20_LastTime = millis() - DS18B20_old;

    if (xSemaphoreX != NULL and DS18B20_LastTime > DS18B20_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        unsigned long DS18B20_time = millis();
        syslog_ng("DS18B20 Start " + fFTS(DS18B20_LastTime - DS18B20_Repeat, 0) + "ms");
        
        
          sens18b20.begin();
          sens18b20.requestTemperatures();

          unsigned long cont;
          float ds0 = -127;
          for (cont = 0;  ( ds0 == -127 or ds0 == 85) and cont < 100; cont++)
          {
            vTaskDelay(1);
            ds0 = sens18b20.getTempCByIndex(0);
            //sens18b20.begin();
          }
          if (ds0 and ds0 != -127 and ds0 != 85 and ds0 != -255)
            RootTempRM.add(ds0);
         // else
            //oneWire.reset();
            //delay(1000);
            //sens18b20.begin();

          syslog_ng("DS18B20:" + fFTS(ds0, 3));
          if (cont > 1)
            syslog_ng("DS18B20 Error cont:" + fFTS(cont - 1, 0));

          RootTemp = RootTempRM.getAverage();
          syslog_ng("DS18B20 Filter:" + fFTS(RootTemp, 3));

        DS18B20_time = millis() - DS18B20_time;

        syslog_ng("DS18B20 " + fFTS(DS18B20_time, 0) + "ms end.");
        DS18B20_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
      
    }
    
  }
  syslog_ng("DS18B20 EXIT Task");
}

#endif // c_DS18B2