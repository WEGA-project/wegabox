#if c_MCP3421 == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
  ;

Wire.requestFrom(MCP3421addr, (uint8_t)1);
if (Wire.available())
{

  MCP342x::generalCallReset();
  delay(1); // MC342x needs 300us to settle

  xTaskCreate(TaskMCP3421, "TaskMCP3421", 5000, NULL, 1, NULL);
  syslog_ng("MCP3421: add Task");
}
else
  syslog_err("MCP3421: not detected");

xSemaphoreGive(xSemaphoreX);
#endif // c_MCP3421