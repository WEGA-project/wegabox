// Измерение фоторезистора
#if c_PR == 1
void TaskPR(void *parameters)
{
  pinMode(PR_AnalogPort, INPUT);
  
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(PR_AnalogPort, ADC_ATTEN_DB_11);

  for (;;)
  {
    if (xSemaphore != NULL)
    {
      if (xSemaphoreTake(xSemaphore, (TickType_t)10) == pdTRUE)
      {
    
          PRRM.add ( adc1_get_raw(PR_AnalogPort));
          PR = PRRM.getMedian();
      
          xSemaphoreGive(xSemaphore);
      }
    } 
   vTaskDelay(5000 / portTICK_PERIOD_MS); 
  }
}
#endif //  c_PR