// Измерение фоторезистора
#if c_PR == 1
void TaskPR(void *parameters)
{
  for (;;)
  {if (OtaStart == true) vTaskDelete(NULL);
    if (xSemaphoreX != NULL)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        PRRM.add(adc1_get_raw(PR_AnalogPort));
        PR = PRRM.getMedian();
        xSemaphoreGive(xSemaphoreX);
      }
    }
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
#endif //  c_PR