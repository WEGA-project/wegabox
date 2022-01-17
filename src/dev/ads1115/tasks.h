#if c_ADS1115 == 1
void TaskADS1115(void *parameters)
{
  for (;;)
  {

    if (OtaStart == true)
      vTaskDelete(NULL);

    if (xSemaphoreX != NULL and millis() - ADS1115_old > ADS1115_Repeat)
    {

      if (xSemaphoreTake(xSemaphoreX, (TickType_t)10) == pdTRUE)
      {
        syslog.log(LOG_INFO, fFTS((millis() - ADS1115_old), 0) + " ADS1115 REPEAT");
        syslog.log(LOG_INFO, fFTS(millis() / 1000, 3) + " sec ADS1115 Start");
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

        xSemaphoreGive(xSemaphoreX);
        ADS1115_old = millis();

        syslog.log(LOG_INFO, fFTS(millis() / 1000, 3) + " sec ADS1115 " + fFTS(pHmV, 3));
        syslog.log(LOG_INFO, fFTS(millis() / 1000, 3) + " sec ADS1115 End");
      }
    }

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
#endif // c_ADS1115