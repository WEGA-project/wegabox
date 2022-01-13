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
        PR = PRRM.getMedian();
        xSemaphoreGive(xSemaphore);
      }
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}
#endif //  c_PR