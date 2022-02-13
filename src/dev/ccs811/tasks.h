#if c_CCS811 == 1
void TaskCCS811(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(1000);

    unsigned long CCS811_LastTime = millis() - CCS811_old;

    if (xSemaphoreX != NULL and CCS811_LastTime > CCS811_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long CCS811_time = millis();
        syslog_ng("CCS811 Start " + fFTS(CCS811_LastTime - CCS811_Repeat, 0) + "ms");
      // Read
      ccs811.set_envdata_Celsius_percRH(AirTemp, AirHum);
      uint16_t eco2, etvoc, errstat, raw;
      ccs811.read(&eco2, &etvoc, &errstat, &raw);

      // Print measurement results based on status
      if (errstat == CCS811_ERRSTAT_OK)
      {
        CCS811_eCO2RM.add(eco2);
        CCS811_tVOCRM.add(etvoc);
        eRAW = raw;
        CO2 = CCS811_eCO2RM.getAverage();
        tVOC = CCS811_tVOCRM.getAverage();
      }
       

        syslog_ng("CCS811 CO2:" + fFTS(eco2, 3));
        syslog_ng("CCS811 tVOC:" + fFTS(etvoc, 3));
        syslog_ng("CCS811 raw:" + fFTS(raw, 3));

        CCS811_time = millis() - CCS811_time;

        syslog_ng("CCS811 " + fFTS(CCS811_time, 0) + "ms end.");
        CCS811_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif