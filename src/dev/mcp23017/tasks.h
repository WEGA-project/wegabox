#if c_MCP23017 == 1

void TaskMCP23017(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(1000);

    unsigned long MCP23017_LastTime = millis() - MCP23017_old;

    if (xSemaphoreX != NULL and MCP23017_LastTime > MCP23017_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long MCP23017_time = millis();
        syslog_ng("MCP23017 Start " + fFTS(MCP23017_LastTime - MCP23017_Repeat, 0) + "ms");

        pwd_val = preferences.getInt("PWD", 0);
        pwd_freq = preferences.getInt("FREQ", 0);
        pwd_port = preferences.getInt("PWDport", 0);

        syslog_ng("MCP23017 PWD PORT:" + fFTS(pwd_port, 0));
        syslog_ng("MCP23017 PWD VALUE:" + fFTS(pwd_val, 0));
        syslog_ng("MCP23017 PWD FREQ:" + fFTS(pwd_freq, 0));

        if (pwd_freq != 0 and pwd_port != 0)
        {
          const int ledChannel = 0;
          const int resolution = 8;
          ledcSetup(ledChannel, pwd_freq, resolution);
          ledcAttachPin(pwd_port, ledChannel);
          ledcWrite(ledChannel, pwd_val);
        }

        int MCP23017_p0 = preferences.getInt("MCP23017_p0", 0);
        if (MCP23017_p0 == 1)
        {
          mcp.pinMode(0, OUTPUT);
          mcp.digitalWrite(0, HIGH);
          syslog_ng("MCP23017 P0: On");
        }
        else
        {
          mcp.digitalWrite(0, LOW);
          syslog_ng("MCP23017 P0: Off");
        }

        int MCP23017_p1 = preferences.getInt("MCP23017_p1", 0);
        if (MCP23017_p1 == 1)
        {
          mcp.pinMode(1, OUTPUT);
          mcp.digitalWrite(1, HIGH);
          syslog_ng("MCP23017 P1: On");
        }
        else
        {
          mcp.digitalWrite(1, LOW);
          syslog_ng("MCP23017 P1: Off");
        }

        int MCP23017_p2 = preferences.getInt("MCP23017_p2", 0);
        if (MCP23017_p2 == 1)
        {
          mcp.pinMode(2, OUTPUT);
          mcp.digitalWrite(2, HIGH);
          syslog_ng("MCP23017 P2: On");
        }
        else
        {
          mcp.digitalWrite(2, LOW);
          syslog_ng("MCP23017 P2: Off");
        }

        int MCP23017_p3 = preferences.getInt("MCP23017_p3", 0);
        if (MCP23017_p3 == 1)
        {
          mcp.pinMode(3, OUTPUT);
          mcp.digitalWrite(3, HIGH);
          syslog_ng("MCP23017 P3: On");
        }
        else
        {
          mcp.digitalWrite(3, LOW);
          syslog_ng("MCP23017 P3: Off");
        }

        vTaskDelay(10000);

        syslog_ng("MCP23017 End ");

        MCP23017_time = millis() - MCP23017_time;

        syslog_ng("MCP23017 " + fFTS(MCP23017_time, 0) + "ms end.");
        MCP23017_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
  syslog_ng("MCP23017 EXIT Task");
}

#endif // c_MCP23017