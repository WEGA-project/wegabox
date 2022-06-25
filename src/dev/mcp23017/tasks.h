#if c_MCP23017 == 1

void TaskMCP23017(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(100);

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
          float setEC = preferences.getFloat("ECStabValue", 2.5);
          int setTime = preferences.getInt("ECStabTime", 20);
          if (wEC > setEC)
          {
            syslog_ng("EC Stab: EC=" + fFTS(wEC, 3) + " > EC max=" + fFTS(setEC, 3) + " ECStab pomp power up");

            mcp.digitalWrite(DRV1_D, 1);
            delay(setTime * 1000);
            mcp.digitalWrite(DRV1_D, 0);
            syslog_ng("EC Stab: ECStab pomp power down");
          }
          else
            mcp.digitalWrite(DRV1_D, 0);
        }

        // Остановка помпы ночью по датчику освещенности (если темно то отключить, если светло включить)
        if (preferences.getInt("PompNightEnable", -1) == 1 and PR != -1)
        {
          float PompNightLightLevel = preferences.getFloat("PompNightLightLevel", 0);
          String PompNightPomp = preferences.getString("PompNightPomp", "DRV1_A");
          if (PompNightLightLevel < PR)
          {
            preferences.putInt((PompNightPomp + "_State").c_str(), 1);
            syslog_ng("PompNight " + PompNightPomp + ": UP PR=" + fFTS(PR, 3));
          }
          else
          {
            preferences.putInt((PompNightPomp + "_State").c_str(), 0);
            syslog_ng("PompNight " + PompNightPomp + ": DOWN PR=" + fFTS(PR, 3));
          }
        }

        // Параметры шимов
        const int PwdChannel1 = 1;
        const int PwdResolution1 = 8;
        const int PwdChannel2 = 2;
        const int PwdResolution2 = 8;

        // KickUp пнуть шим насоса до предела перед снижением оборотов до нормы при включении
        int PompKickUP = 0;     // Пинок для насоса
        int KickUpMax = 254;    // максимум мощности пинка
        int KickUpStrart = 10; // начальная можность пинка
        int KickUpTime = 300;   // Время пинка в миллисекундах
        if (pwd_val != preferences.getInt("PWD", 0) or pwd_freq != preferences.getInt("FREQ", 0) or pwd_port != preferences.getInt("PWDport", 0))

        {

          if (preferences.getInt("PWD", 0) > pwd_val or !pwd_val)
            PompKickUP = 1;

          pwd_val = preferences.getInt("PWD", 0);
          pwd_freq = preferences.getInt("FREQ", 0);
          pwd_port = preferences.getInt("PWDport", 0);

          syslog_ng("MCP23017 PWD PORT:" + String(pwd_port));
          syslog_ng("MCP23017 PWD VALUE:" + String(pwd_val));
          syslog_ng("MCP23017 PWD FREQ:" + String(pwd_freq));
          lcd("PWD1-" + String(pwd_freq) + "Hz:" + String(pwd_val));

          ledcSetup(PwdChannel1, pwd_freq, PwdResolution1);
          ledcAttachPin(pwd_port, PwdChannel1);
          if (PompKickUP == 1)
            PwdPompKick(PwdChannel1, KickUpMax, KickUpStrart, pwd_val, KickUpTime);
          ledcWrite(PwdChannel1, pwd_val);
        }

        pwd_val2 = preferences.getInt("PWD2", 0);
        pwd_freq2 = preferences.getInt("FREQ2", 0);
        pwd_port2 = preferences.getInt("PWDport2", 0);

        syslog_ng("MCP23017 PWD2 PORT:" + fFTS(pwd_port2, 0));
        syslog_ng("MCP23017 PWD2 VALUE:" + fFTS(pwd_val2, 0));
        syslog_ng("MCP23017 PWD2 FREQ:" + fFTS(pwd_freq2, 0));

        if (pwd_freq2 != 0 and pwd_port2 != 0)
        {
          ledcSetup(PwdChannel2, pwd_freq2, PwdResolution2);
          ledcAttachPin(pwd_port2, PwdChannel2);
          ledcWrite(PwdChannel2, pwd_val2);
        }
        // readGPIO_0 = mcp.readGPIO(0);
        // readGPIO_1 = mcp.readGPIO(1);
        readGPIO=mcp.readGPIOAB();
        //int ms=10;

        mcp.digitalWrite(DRV1_A, preferences.getInt("DRV1_A_State", 0));
        if (bitRead(readGPIO, DRV1_A) == 0 and preferences.getInt("DRV1_A_State", 0) == 1)
          PwdPompKick(PwdChannel1, KickUpMax, KickUpStrart, pwd_val, KickUpTime);
        syslog_ng("MCP23017 DRV1_A (" + fFTS(DRV1_A, 0) + "):" + fFTS(preferences.getInt("DRV1_A_State", 0), 0));

        mcp.digitalWrite(DRV1_B, preferences.getInt("DRV1_B_State", 0));
        if (bitRead(readGPIO, DRV1_B) == 0 and preferences.getInt("DRV1_B_State", 0) == 1)
          PwdPompKick(PwdChannel1, KickUpMax, KickUpStrart, pwd_val, KickUpTime);
        syslog_ng("MCP23017 DRV1_B (" + fFTS(DRV1_B, 0) + "):" + fFTS(preferences.getInt("DRV1_B_State", 0), 0));

        mcp.digitalWrite(DRV1_C, preferences.getInt("DRV1_C_State", 0));
        if (bitRead(readGPIO, DRV1_C) == 0 and preferences.getInt("DRV1_C_State", 0) == 1)
          PwdPompKick(PwdChannel2, KickUpMax, KickUpStrart, pwd_val, KickUpTime);
        syslog_ng("MCP23017 DRV1_C (" + fFTS(DRV1_C, 0) + "):" + fFTS(preferences.getInt("DRV1_C_State", 0), 0));

        mcp.digitalWrite(DRV1_D, preferences.getInt("DRV1_D_State", 0));
        if (bitRead(readGPIO, DRV1_D) == 0 and preferences.getInt("DRV1_D_State", 0) == 1)
          PwdPompKick(PwdChannel2, KickUpMax, KickUpStrart, pwd_val, KickUpTime);
        syslog_ng("MCP23017 DRV1_D (" + fFTS(DRV1_D, 0) + "):" + fFTS(preferences.getInt("DRV1_D_State", 0), 0));

        mcp.digitalWrite(DRV2_A, preferences.getInt("DRV2_A_State", 0));
        mcp.digitalWrite(DRV2_B, preferences.getInt("DRV2_B_State", 0));
        mcp.digitalWrite(DRV2_C, preferences.getInt("DRV2_C_State", 0));
        mcp.digitalWrite(DRV2_D, preferences.getInt("DRV2_D_State", 0));
        mcp.digitalWrite(DRV3_A, preferences.getInt("DRV3_A_State", 0));
        mcp.digitalWrite(DRV3_B, preferences.getInt("DRV3_B_State", 0));
        mcp.digitalWrite(DRV3_C, preferences.getInt("DRV3_C_State", 0));
        mcp.digitalWrite(DRV3_D, preferences.getInt("DRV3_D_State", 0));
        mcp.digitalWrite(DRV4_A, preferences.getInt("DRV4_A_State", 0));
        mcp.digitalWrite(DRV4_B, preferences.getInt("DRV4_B_State", 0));
        mcp.digitalWrite(DRV4_C, preferences.getInt("DRV4_C_State", 0));
        mcp.digitalWrite(DRV4_D, preferences.getInt("DRV4_D_State", 0));

        // readGPIO_0 = mcp.readGPIO(0);
        // readGPIO_1 = mcp.readGPIO(1);
        readGPIO=mcp.readGPIOAB();

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