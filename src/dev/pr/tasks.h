// Измерение фоторезистора
#if c_PR == 1
void TaskPR(void *parameters)
{
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

        long PR0=adc1_get_raw(PR_AnalogPort);
        PRRM.add(PR0);

        syslog_ng("PR:" + fFTS(PR0, 3));

        PR = PRRM.getMedian();


        PR_time = millis() - PR_time;
        

        syslog_ng("PR " + fFTS(PR_time, 0) + "ms end.");
        PR_old = millis();
        xSemaphoreGive(xSemaphoreX);
         syslog_ng("PR Semaphore Give");
      }
    }
           
  }
}
#endif //  c_PR