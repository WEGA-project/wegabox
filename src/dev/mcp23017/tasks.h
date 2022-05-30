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

        // Адаптивная циркуляция для снижения скачков корневого давления
        if (preferences.getInt("RootPomp", -1) == 1)
        {
          pwd_val = preferences.getInt("PWD", 254);
          RootPwdMax = preferences.getInt("RootPwdMax", 254);
          RootPwdMin = preferences.getInt("RootPwdMin", 0);
          RootDistMin = preferences.getInt("RootDistMin", 6);

          if (RootTemp > AirTemp and RootTemp > 15 and Dist > RootDistMin)
          {
            // preferences.putInt("DRV1_A_State", 0);
            syslog_ng("Root pomp controll: RootTemp=" + fFTS(RootTemp, 3) + " > AirTemp=" + fFTS(AirTemp, 3) + " Root pomp power down");
            pwd_val = pwd_val - 2;
          }
          else
          {

            // preferences.putInt("DRV1_A_State", 1);
            syslog_ng("Root pomp controll: RootTemp=" + fFTS(RootTemp, 3) + " < AirTemp=" + fFTS(AirTemp, 3) + " Root pomp power up");
            pwd_val = pwd_val + 1;
          }

          if (pwd_val < RootPwdMin)
            pwd_val = RootPwdMin;
          if (pwd_val > RootPwdMax)
            pwd_val = RootPwdMax;
          preferences.putInt("PWD", pwd_val);
        }



        // Параметры портов
        long DRV1_A = preferences.getInt("DRV1_A", -1);
        long DRV1_B = preferences.getInt("DRV1_B", -1);
        long DRV1_C = preferences.getInt("DRV1_C", -1);
        long DRV1_D = preferences.getInt("DRV1_D", -1);

        long DRV2_A = preferences.getInt("DRV2_A", -1);
        long DRV2_B = preferences.getInt("DRV2_B", -1);
        long DRV2_C = preferences.getInt("DRV2_C", -1);
        long DRV2_D = preferences.getInt("DRV2_D", -1);

        long DRV3_A = preferences.getInt("DRV3_A", -1);
        long DRV3_B = preferences.getInt("DRV3_B", -1);
        long DRV3_C = preferences.getInt("DRV3_C", -1);
        long DRV3_D = preferences.getInt("DRV3_D", -1);

        long DRV4_A = preferences.getInt("DRV4_A", -1);
        long DRV4_B = preferences.getInt("DRV4_B", -1);
        long DRV4_C = preferences.getInt("DRV4_C", -1);
        long DRV4_D = preferences.getInt("DRV4_D", -1);

                // Коррекция ЕС путем разбавления
        if (preferences.getInt("ECStabEnable", -1) == 1)
        {
          float setEC=preferences.getFloat("ECStabValue", 2.5);
          if (wEC>setEC){
            syslog_ng("EC Stab: EC=" + fFTS(wEC, 3) + " > EC max=" + fFTS(setEC, 3) + " ECStab pomp power up");
            mcp.digitalWrite(DRV1_D, 1);
            delay (20000);
            mcp.digitalWrite(DRV1_D, 0);
            syslog_ng("EC Stab: ECStab pomp power down");
          }
          else
          mcp.digitalWrite(DRV1_D, 0);
        }

        // Параметры шимов
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
          const int ledChannel = 1;
          const int resolution = 8;
          ledcSetup(ledChannel, pwd_freq, resolution);
          ledcAttachPin(pwd_port, ledChannel);
          ledcWrite(ledChannel, pwd_val);
        }

        if (pwd_freq2 != 0 and pwd_port2 != 0)
        {
          const int ledChannel2 = 2;
          const int resolution2 = 8;
          ledcSetup(ledChannel2, pwd_freq2, resolution2);
          ledcAttachPin(pwd_port2, ledChannel2);
          ledcWrite(ledChannel2, pwd_val2);
        }

        mcp.digitalWrite(DRV1_A, preferences.getInt("DRV1_A_State", 0));
        syslog_ng("MCP23017 DRV1_A (" + fFTS(DRV1_A, 0) + "):" + fFTS(preferences.getInt("DRV1_A_State", 0), 0));

        mcp.digitalWrite(DRV1_B, preferences.getInt("DRV1_B_State", 0));
        syslog_ng("MCP23017 DRV1_B (" + fFTS(DRV1_B, 0) + "):" + fFTS(preferences.getInt("DRV1_B_State", 0), 0));

        mcp.digitalWrite(DRV1_C, preferences.getInt("DRV1_C_State", 0));
        syslog_ng("MCP23017 DRV1_C (" + fFTS(DRV1_C, 0) + "):" + fFTS(preferences.getInt("DRV1_C_State", 0), 0));

        mcp.digitalWrite(DRV1_D, preferences.getInt("DRV1_D_State", 0));
        syslog_ng("MCP23017 DRV1_D (" + fFTS(DRV1_D, 0) + "):" + fFTS(preferences.getInt("DRV1_D_State", 0), 0));

        vTaskDelay(MCP23017_Repeat);

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