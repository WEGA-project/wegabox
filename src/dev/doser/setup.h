#if c_MCP23017 == 1 and c_DOSER == 1

while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
  ;


  mcp.writeGPIOAB(0);
  xTaskCreate(TaskDOSER, "TaskDOSER", 5000, NULL, 1, NULL);
  syslog_ng("DOSER: add Task");

xSemaphoreGive(xSemaphoreX);


#endif // c_DOSER