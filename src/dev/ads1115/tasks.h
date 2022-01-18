#if c_ADS1115 == 1
void TaskADS1115(void *parameters)
{
  for (;;)
  { //
    if (OtaStart == true)
      vTaskDelete(NULL);

    unsigned long ADS1115_LastTime = millis() - ADS1115_old;

    if (xSemaphoreX != NULL and ADS1115_LastTime > ADS1115_Repeat)
    { //syslog_ng("ADS1115 Semaphore");
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long ADS1115_time = millis();
        syslog_ng("ADS1115 Start " + fFTS(ADS1115_LastTime - ADS1115_Repeat, 0) + "ms");

        // long cont = 0;
        //     double sensorValue = 0;

        rtc_wdt_protect_off();
        rtc_wdt_disable();
        disableCore0WDT();
        disableLoopWDT();
        // for (unsigned long cont=0;cont<ADS1115_MiddleCount and OtaStart != true;cont++){
        //   sensorValue = adc.getResult_mV() + sensorValue;

        // }
        while (adc.isBusy() == true)
          vTaskDelay(1);

        pHraw = adc.getRawResult();
        rtc_wdt_protect_on();
        rtc_wdt_enable();
        enableCore0WDT();
        enableLoopWDT();

        //pHmV = sensorValue / ADS1115_MiddleCount;
        //PhRM.add(sensorValue / cont);
        //pHmV = PhRM.getMedian();

        ADS1115_time = millis() - ADS1115_time;
        syslog_ng("ADS1115 pHraw:" + fFTS(pHraw, 3));
        syslog_ng("ADS1115 " + fFTS(ADS1115_time, 0) + "ms end.");
        ADS1115_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }

    vTaskDelay(1);
  }
}
#endif // c_ADS1115