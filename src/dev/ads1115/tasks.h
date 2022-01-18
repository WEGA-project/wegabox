#if c_ADS1115 == 1
void TaskADS1115(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    unsigned long ADS1115_LastTime = millis() - ADS1115_old;
    if (xSemaphoreX != NULL and ADS1115_LastTime > ADS1115_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        syslog_ng("ADS1115 Start");
        syslog_ng("ADS1115 Last time old " + fFTS(ADS1115_LastTime - ADS1115_Repeat, 0));
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

        xSemaphoreGive(xSemaphoreX);
        ADS1115_old = millis();
        syslog_ng("ADS1115 End");
      }
    }

    vTaskDelay(10);
  }
}
#endif // c_ADS1115