#if c_MCP23017 == 1

void TaskMCP23017(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);

    unsigned long MCP23017_LastTime = millis() - MCP23017_old;

    if (xSemaphoreX != NULL and MCP23017_LastTime > MCP23017_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long MCP23017_time = millis();
        syslog_ng("MCP23017 Start " + fFTS(MCP23017_LastTime - MCP23017_Repeat, 0) + "ms");

        vTaskDelay(1000);

        if (pwd != preferences.getInt("PWD", 0))
        {
          syslog_ng("PWD change set:" + fFTS(pwd, 0));
          mcp.pinMode(DRV1_A, OUTPUT);

          pwd = preferences.getInt("PWD", 0);
          int freq = preferences.getInt("FREQ", 0);

          ledcSetup(0, freq, 8);
          ledcAttachPin(PWD2, 0);
          ledcWrite(0, pwd);
          mcp.digitalWrite(DRV1_A, HIGH);
        }

        MCP23017_time = millis() - MCP23017_time;
        syslog_ng("MCP23017 " + fFTS(MCP23017_time, 0) + "ms end.");
        MCP23017_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}

#endif // c_MCP23017