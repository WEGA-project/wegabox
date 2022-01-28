#if c_HX710B == 1
void TaskHX710B(void *parameters)
{
  for (;;)
  { //
    if (OtaStart == true)
      vTaskDelete(NULL);
    //syslog_ng("HX710B loop");  
    vTaskDelay(1000);

    unsigned long HX710B_LastTime = millis() - HX710B_old;

    if (xSemaphoreX != NULL and HX710B_LastTime > HX710B_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long HX710B_time = millis();
        syslog_ng("HX710B Start " + fFTS(HX710B_LastTime - HX710B_Repeat, 0) + "ms");

            uint32_t data_raw = 0;
            if ( air_press.read(&data_raw, 1000UL) == HX710B_OK )
            {
              Serial.print(F("Data raw of ADC is : "));
              Serial.println((unsigned long) data_raw);
              //Dist=DstMediana.filtered(data_raw);
              //DstRM.add(data_raw);
              //Dist=DstRM.getMedian();
              Dist=data_raw;
              Serial.println(Dist);
            }
      
        HX710B_time = millis() - HX710B_time;
        syslog_ng("HX710B Pressure RAW:" + fFTS(data_raw, 0));    
        syslog_ng("HX710B " + fFTS(HX710B_time, 0) + "ms end.");
        HX710B_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif //c_HX710B