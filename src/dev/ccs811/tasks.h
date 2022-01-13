#if c_CCS811 == 1
void TaskCCS811(void *parameters)
{
  for (;;)
  {
    if (xSemaphoreTake(xI2CSemaphore, (TickType_t)5) == pdTRUE)
    {
      // Read
      ccs811.set_envdata_Celsius_percRH(AirTemp, AirHum);
      uint16_t eco2, etvoc, errstat, raw;
      ccs811.read(&eco2, &etvoc, &errstat, &raw);

      // Print measurement results based on status
      if (errstat == CCS811_ERRSTAT_OK)
      {
        CO2 = eco2;
        tVOC = etvoc;
        eRAW = raw;
      }
      
      xSemaphoreGive(xI2CSemaphore);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
#endif