#if c_US025 == 1
void TaskUS(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    unsigned long US025_LastTime = millis() - US025_old;
    if (xSemaphoreX != NULL and US025_LastTime > US025_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        syslog_ng("US025 Start");
        syslog_ng("US025 Last time old " + fFTS(US025_LastTime - US025_Repeat, 0));
        float us = distanceSensor.measureDistanceCm(25);
        if (us > 1)
        {
          DstRM.add(us);
          //Serial.println(us);
        }
        Dist = DstRM.getMedian();
        // else
        // {
        //   Serial.print("US-025 Error us=");
        //   Serial.println(us);
        // }
        xSemaphoreGive(xSemaphoreX);
        US025_old = millis();
        syslog_ng("US025 End");
      }
    }
    
        vTaskDelay(10);
  }
}
#endif // c_US025