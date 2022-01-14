#if c_US025 == 1
void TaskUS(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    if (xSemaphore != NULL)
    {
      if (xSemaphoreTake(xSemaphore, (TickType_t)5) == pdTRUE)
      {
        float us = distanceSensor.measureDistanceCm(25);
        if (us > 1)
        {
          DstRM.add(us);
          //Serial.println(us);
        }
        // else
        // {
        //   Serial.print("US-025 Error us=");
        //   Serial.println(us);
        // }
        xSemaphoreGive(xSemaphore);
      }
    }
    Dist = DstRM.getMedian();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
#endif // c_US025