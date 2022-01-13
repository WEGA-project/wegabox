// Измерение фоторезистора
#if c_PR == 1
void TaskPR(void *parameters)
{
  for (;;)
  {
    if (xSemaphore != NULL)
    {
      if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE)
      {
        PRRM.add(adc1_get_raw(PR_AnalogPort));
        
        xSemaphoreGive(xSemaphore);
      }
    }
    PR = PRRM.getMedian();
    vTaskDelay(20000 / portTICK_PERIOD_MS);
  }
}
#endif //  c_PR