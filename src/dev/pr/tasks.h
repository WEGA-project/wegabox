// Измерение фоторезистора
#if c_PR == 1
void TaskPR(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    unsigned long PR_LastTime = millis() - PR_old;
    if (xSemaphoreX != NULL and PR_LastTime > PR_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        syslog_ng("PR Start");
        syslog_ng("PR Last time old " + fFTS(PR_LastTime - PR_Repeat, 0));
        PRRM.add(adc1_get_raw(PR_AnalogPort));
        PR = PRRM.getMedian();
        xSemaphoreGive(xSemaphoreX);
        PR_old = millis();
        syslog_ng("PR End");
      }
    }

       vTaskDelay(10);
  }
}
#endif //  c_PR