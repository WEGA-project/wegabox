#if c_MCP3421 == 1
void TaskMCP3421(void *parameters)
{
  long adcvalue;
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    else
    {
      if (xSemaphoreTake(xI2CSemaphore, (TickType_t)5) == pdTRUE)
      {
        MCP342x::Config status;
        long phvalue;
        uint8_t err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot, MCP342x::resolution18, MCP342x::gain4, 100000, phvalue, status);
        if (!err)
        {

          //pHraw = phvalue;
          //pHmV = PhMediana.filtered(4096 / pow(2, 18) * pHraw / 4);
          pHraw = phvalue;
          PhRM.add(phvalue);
          pHmV=(4096 / pow(2, 18) *PhRM.getMedian()/ 4);
        }
        xSemaphoreGive(xI2CSemaphore);
      }
    vTaskDelay(2000 / portTICK_PERIOD_MS);  
    }
  }
}
#endif // c_MCP3421