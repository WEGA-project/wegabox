// Измерение фоторезистора
#if c_PR == 1
void TaskPR(void *parameters)
{
  pinMode(PR_AnalogPort, INPUT);
  
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(PR_AnalogPort, ADC_ATTEN_DB_11);

  for (;;)
  {
    if (OtaStart == true)
    {
      vTaskDelete(NULL);
    }
    else
    {
          PRRM.add ( adc1_get_raw(PR_AnalogPort));
          PR = PRRM.getMedian();
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
  }
}
#endif //  c_PR