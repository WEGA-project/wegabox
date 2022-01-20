#if c_ADS1115 == 1
void TaskADS1115(void *parameters)
{
  for (;;)
  { //
    if (OtaStart == true)
      vTaskDelete(NULL);
    //syslog_ng("ADS1115 loop");  
    vTaskDelay(1000);

    unsigned long ADS1115_LastTime = millis() - ADS1115_old;

    if (xSemaphoreX != NULL and ADS1115_LastTime > ADS1115_Repeat)
    { //syslog_ng("ADS1115 Semaphore");
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long ADS1115_time = millis();
        syslog_ng("ADS1115 Start " + fFTS(ADS1115_LastTime - ADS1115_Repeat, 0) + "ms");


        // //while (adc.isBusy() == true)
         

        pHraw = adc.getRawResult();


        // //pHmV = sensorValue / ADS1115_MiddleCount;
        // //PhRM.add(sensorValue / cont);
        // //pHmV = PhRM.getMedian();

      
      // adc.setCompareChannels(ADS1115_COMP_0_3);
      // adc.setVoltageRange_mV(ADS1115_RANGE_4096);
      // adc.setConvRate(ADS1115_860_SPS);
      long cont=0;
      double sensorValue=0;
      while ( cont < ADS1115_MiddleCount){
       cont++;
        sensorValue =  adc.getResult_mV()+sensorValue;
      }
      pHmV=sensorValue/cont;

        ADS1115_time = millis() - ADS1115_time;
        syslog_ng("ADS1115 pHmV:" + fFTS(pHmV, 3));
        syslog_ng("ADS1115 " + fFTS(ADS1115_time, 0) + "ms end.");
        ADS1115_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif // c_ADS1115