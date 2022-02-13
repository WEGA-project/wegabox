#if c_US025 == 1
void TaskUS(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(1000);

    unsigned long US025_LastTime = millis() - US025_old;

    if (xSemaphoreX != NULL and US025_LastTime > US025_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long US025_time = millis();
        syslog_ng("US025 Start " + fFTS(US025_LastTime - US025_Repeat, 0) + "ms");

        float us = distanceSensor.measureDistanceCm(25);

        syslog_ng("US025:" + fFTS(us, 3));

        if (us > 1)
        {
          DstRM.add(us);
          //Serial.println(us);
        }
        Dist = DstRM.getAverage(4);

        US025_time = millis() - US025_time;        
        syslog_ng("US025 " + fFTS(US025_time, 0) + "ms end.");
        US025_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif // c_US025