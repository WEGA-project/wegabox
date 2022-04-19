#if c_CPUTEMP == 1
void TaskCPUTEMP(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(1000);

    unsigned long CPUTEMP_LastTime = millis() - CPUTEMP_old;

    if (xSemaphoreX != NULL and CPUTEMP_LastTime > CPUTEMP_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long CPUTEMP_time = millis();
        syslog_ng("CPUTEMP Start " + fFTS(CPUTEMP_LastTime - CPUTEMP_Repeat, 0) + "ms");
      for (long i=0;i<50;i++)     CpuTempRM.add((readTemp1(true) + readTemp2(true)) /2) ;

        CPUTemp = CpuTempRM.getAverage();
        syslog_ng("CPUTemp-Core1:" + fFTS(readTemp1(true), 3));
        syslog_ng("CPUTemp-Core2:" + fFTS(readTemp2(true), 3));
        syslog_ng("CPUTemp:" + fFTS(CPUTemp, 3));

        CPUTEMP_time = millis() - CPUTEMP_time;

        syslog_ng("CPUTEMP " + fFTS(CPUTEMP_time, 0) + "ms end.");
        CPUTEMP_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif //c_CPUTEMP