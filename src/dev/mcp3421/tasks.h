#if c_MCP3421 == 1
void TaskMCP3421(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    delay(100);

    unsigned long MCP3421_LastTime = millis() - MCP3421_old;

    if (xSemaphoreX != NULL and MCP3421_LastTime > MCP3421_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        unsigned long MCP3421_time = millis();
        syslog_ng("MCP3421 Start " + fFTS(MCP3421_LastTime - MCP3421_Repeat, 0) + "ms");
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
        syslog_ng("MCP3421 RAW:" + fFTS(pHraw, 3));
        syslog_ng("MCP3421 pHmV:" + fFTS(pHmV, 3));

        MCP3421_time = millis() - MCP3421_time;

        syslog_ng("MCP3421 " + fFTS(MCP3421_time, 0) + "ms end.");
        MCP3421_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif // c_MCP3421