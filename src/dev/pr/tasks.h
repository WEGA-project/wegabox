// Измерение фоторезистора
#if c_PR == 1
void TaskPR(void *parameters)
{
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(PR_AnalogPort, ADC_ATTEN_DB_11);
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(1000);

    unsigned long PR_LastTime = millis() - PR_old;

    if (xSemaphoreX != NULL and PR_LastTime > PR_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long PR_time = millis();
        syslog_ng("PR Start " + fFTS(PR_LastTime - PR_Repeat, 0) + "ms");

       

        //if(PR0) PR=PR0; else PR=-1;
        //PRRM.add(PR0);
        long cont;
        long PR0=0;
      for (cont=0;cont<PR_MiddleCount;cont++){
      PR0=adc1_get_raw(PR_AnalogPort)+PR0;        
      }
      PR=float(PR0)/cont;
syslog_ng("PR:" + fFTS(PR, 3));

        //PR = PRRM.getMedian();


        PR_time = millis() - PR_time;
        

        syslog_ng("PR " + fFTS(PR_time, 0) + "ms end.");
        PR_old = millis();
        xSemaphoreGive(xSemaphoreX);
         //syslog_ng("PR Semaphore Give");
      }
    }
         
  }
}
#endif //  c_PR