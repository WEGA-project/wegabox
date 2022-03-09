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


      long DRV1_A=preferences.getInt("DRV1_A", -1);
      long DRV1_B=preferences.getInt("DRV1_B", -1);
      long DRV1_C=preferences.getInt("DRV1_C", -1);
      long DRV1_D=preferences.getInt("DRV1_D", -1);

      long DRV2_A=preferences.getInt("DRV2_A", -1);
      long DRV2_B=preferences.getInt("DRV2_B", -1);
      long DRV2_C=preferences.getInt("DRV2_C", -1);
      long DRV2_D=preferences.getInt("DRV2_D", -1);
      
      long DRV3_A=preferences.getInt("DRV3_A", -1);
      long DRV3_B=preferences.getInt("DRV3_B", -1);
      long DRV3_C=preferences.getInt("DRV3_C", -1);
      long DRV3_D=preferences.getInt("DRV3_D", -1);

      long DRV4_A=preferences.getInt("DRV4_A", -1);
      long DRV4_B=preferences.getInt("DRV4_B", -1);
      long DRV4_C=preferences.getInt("DRV4_C", -1);
      long DRV4_D=preferences.getInt("DRV4_D", -1);


        pwd_val = preferences.getInt("PWD", 0);
        pwd_freq = preferences.getInt("FREQ", 0);
        pwd_port = preferences.getInt("PWDport", 0);

        syslog_ng("MCP23017 PWD PORT:" + fFTS(pwd_port, 0));
        syslog_ng("MCP23017 PWD VALUE:" + fFTS(pwd_val, 0));
        syslog_ng("MCP23017 PWD FREQ:" + fFTS(pwd_freq, 0));

        pwd_val2 = preferences.getInt("PWD2", 0);
        pwd_freq2 = preferences.getInt("FREQ2", 0);
        pwd_port2 = preferences.getInt("PWDport2", 0);

        syslog_ng("MCP23017 PWD2 PORT:" + fFTS(pwd_port2, 0));
        syslog_ng("MCP23017 PWD2 VALUE:" + fFTS(pwd_val2, 0));
        syslog_ng("MCP23017 PWD2 FREQ:" + fFTS(pwd_freq2, 0));

        if (pwd_freq != 0 and pwd_port != 0)
        {
          const int ledChannel = 0;
          const int resolution = 8;
          ledcSetup(ledChannel, pwd_freq, resolution);
          ledcAttachPin(pwd_port, ledChannel);
          ledcWrite(ledChannel, pwd_val);
        }

        if (pwd_freq2 != 0 and pwd_port2 != 0)
        {
          const int ledChannel2 = 0;
          const int resolution2 = 8;
          ledcSetup(ledChannel2, pwd_freq2, resolution2);
          ledcAttachPin(pwd_port2, ledChannel2);
          ledcWrite(ledChannel2, pwd_val2);
        }


        mcp.digitalWrite(DRV1_A, preferences.getInt("DRV1_A_State", 0));
        syslog_ng("MCP23017 DRV1_A ("+fFTS(DRV1_A,0)+"):"+fFTS(preferences.getInt("DRV1_A_State", 0),0));

        // int MCP23017_p0 = preferences.getInt("MCP23017_p0", 0);
        // if (MCP23017_p0 == 1)
        // {
        //   mcp.pinMode(0, OUTPUT);
        //   mcp.digitalWrite(0, HIGH);
        //   syslog_ng("MCP23017 P0: On");
        // }
        // else
        // {
        //   mcp.digitalWrite(0, LOW);
        //   syslog_ng("MCP23017 P0: Off");
        // }

        // int MCP23017_p1 = preferences.getInt("MCP23017_p1", 0);
        // if (MCP23017_p1 == 1)
        // {
        //   mcp.pinMode(1, OUTPUT);
        //   mcp.digitalWrite(1, HIGH);
        //   syslog_ng("MCP23017 P1: On");
        // }
        // else
        // {
        //   mcp.digitalWrite(1, LOW);
        //   syslog_ng("MCP23017 P1: Off");
        // }

        // int MCP23017_p2 = preferences.getInt("MCP23017_p2", 0);
        // if (MCP23017_p2 == 1)
        // {
        //   mcp.pinMode(2, OUTPUT);
        //   mcp.digitalWrite(2, HIGH);
        //   syslog_ng("MCP23017 P2: On");
        // }
        // else
        // {
        //   mcp.digitalWrite(2, LOW);
        //   syslog_ng("MCP23017 P2: Off");
        // }

        // int MCP23017_p3 = preferences.getInt("MCP23017_p3", 0);
        // if (MCP23017_p3 == 1)
        // {
        //   mcp.pinMode(3, OUTPUT);
        //   mcp.digitalWrite(3, HIGH);
        //   syslog_ng("MCP23017 P3: On");
        // }
        // else
        // {
        //   mcp.digitalWrite(3, LOW);
        //   syslog_ng("MCP23017 P3: Off");
        // }

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