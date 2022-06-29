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

        mcp.begin_I2C();

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

        // Параметры шимов
        const int PwdChannel1 = 1;
        const int PwdResolution1 = 8;
        const int PwdChannel2 = 2;
        const int PwdResolution2 = 8;

        // KickUp пнуть шим до предела перед снижением до нормы при включении
        int PompKickUP = 0;    // Пинок для насоса
        int KickUpMax = 254;   // максимум мощности пинка
        int KickUpStrart = 10; // начальная можность пинка
        int KickUpTime = 300;  // Время пинка в миллисекундах

        // PWD 1
        if (pwd_val != preferences.getInt("PWD", 0) 
        or pwd_freq != preferences.getInt("FREQ", 0) 
        or pwd_port != preferences.getInt("PWDport", 0))

        {

          if (preferences.getInt("PWD", 0) > pwd_val or !pwd_val)
            PompKickUP = 1;

          pwd_val = preferences.getInt("PWD", 0);
          pwd_freq = preferences.getInt("FREQ", 0);
          pwd_port = preferences.getInt("PWDport", 0);

          syslog_ng("MCP23017 PWD1 PORT:" + String(pwd_port));
          syslog_ng("MCP23017 PWD1 VALUE:" + String(pwd_val));
          syslog_ng("MCP23017 PWD1 FREQ:" + String(pwd_freq));
          lcd("PWD1-" + String(pwd_freq) + "Hz:" + String(pwd_val));

          ledcSetup(PwdChannel1, pwd_freq, PwdResolution1);
          ledcAttachPin(pwd_port, PwdChannel1);
          if (PompKickUP == 1)
            PwdPompKick(PwdChannel1, KickUpMax, KickUpStrart, pwd_val, KickUpTime);
          ledcWrite(PwdChannel1, pwd_val);
        }

        //PWD 2
        if (pwd_val2 != preferences.getInt("PWD2", 0) or pwd_freq2 != preferences.getInt("FREQ2", 0) or pwd_port2 != preferences.getInt("PWDport2", 0))

        {

          if (preferences.getInt("PWD2", 0) > pwd_val2 or !pwd_val2)
            PompKickUP = 1;

          pwd_val2 = preferences.getInt("PWD2", 0);
          pwd_freq2 = preferences.getInt("FREQ2", 0);
          pwd_port2 = preferences.getInt("PWDport2", 0);

          syslog_ng("MCP23017 PWD2 PORT:" + String(pwd_port2));
          syslog_ng("MCP23017 PWD2 VALUE:" + String(pwd_val2));
          syslog_ng("MCP23017 PWD2 FREQ:" + String(pwd_freq2));
          lcd("PWD2-" + String(pwd_freq2) + "Hz:" + String(pwd_val2));

          ledcSetup(PwdChannel2, pwd_freq2, PwdResolution2);
          ledcAttachPin(pwd_port2, PwdChannel2);
          if (PompKickUP == 1)
            PwdPompKick(PwdChannel2, KickUpMax, KickUpStrart, pwd_val2, KickUpTime);
          ledcWrite(PwdChannel2, pwd_val2);
        }





        // Адаптивная циркуляция для снижения скачков корневого давления
        int pwd_val_root;
        if (preferences.getInt("RootPomp", -1) == 1)
        {
          pwd_val_root = preferences.getInt("PWD", 254);
          RootPwdMax = preferences.getInt("RootPwdMax", 254);
          RootPwdMin = preferences.getInt("RootPwdMin", 0);
          RootDistMin = preferences.getInt("RootDistMin", 6);

          if (RootTemp > AirTemp and RootTemp > 15 and Dist > RootDistMin)
          {
            // preferences.putInt("DRV1_A_State", 0);
            syslog_ng("Root pomp controll: RootTemp=" + fFTS(RootTemp, 3) + " > AirTemp=" + fFTS(AirTemp, 3) + " Root pomp power down");
            pwd_val_root = pwd_val_root - 2;
          }
          else
          {

            // preferences.putInt("DRV1_A_State", 1);
            syslog_ng("Root pomp controll: RootTemp=" + fFTS(RootTemp, 3) + " < AirTemp=" + fFTS(AirTemp, 3) + " Root pomp power up");
            pwd_val_root = pwd_val_root + 1;
          }

          if (pwd_val_root < RootPwdMin)
            pwd_val_root = RootPwdMin;
          if (pwd_val_root > RootPwdMax)
            pwd_val_root = RootPwdMax;
          preferences.putInt("PWD", pwd_val_root);
        }

        // Коррекция ЕС путем разбавления
        if (preferences.getInt("ECStabEnable", -1) == 1)
        {
          float setEC = preferences.getFloat("ECStabValue", 2.5);
          int setTime = preferences.getInt("ECStabTime", 20);
          String ECStabPomp = preferences.getString("ECStabPomp", "DRV1_D");
          int DRV = preferences.getInt(ECStabPomp.c_str(), -1);
          int ECStabInterval = preferences.getInt("ECStabInterval", 180);
          float ECStabCriticalLevel = preferences.getFloat("ECStabCriticalLevel", 5);

          if (wEC > setEC and millis() - ECStabTimeStart > ECStabInterval * 1000 and Dist >= ECStabCriticalLevel)
          {

            syslog_ng("EC Stab: EC=" + fFTS(wEC, 3) + " > EC max=" + fFTS(setEC, 3) + " ECStab pomp:" + ECStabPomp + " power up on " + String(setTime) + " sec");
            mcp.digitalWrite(DRV, 1);
            delay(setTime * 1000);
            mcp.digitalWrite(DRV, 0);
            syslog_ng("EC Stab: ECStab pomp:" + ECStabPomp + " power down");
            ECStabTimeStart = millis();
          }
          else
            syslog_ng("EC Stab: EC=" + fFTS(wEC, 3) + " ECStab pomp:" + ECStabPomp + " power Off. Time old:" + fFTS((millis() - ECStabTimeStart) / 1000, 0) + " sec. Dist=" + fFTS(Dist, 3) + "cm");
          mcp.digitalWrite(DRV, 0);
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



        readGPIO = mcp.readGPIOAB();

        mcp.digitalWrite(DRV1_A, preferences.getInt("DRV1_A_State", 0));
        if (bitRead(readGPIO, DRV1_A) == 0 and preferences.getInt("DRV1_A_State", 0) == 1)
          PwdPompKick(PwdChannel1, KickUpMax, KickUpStrart, pwd_val, KickUpTime);

        mcp.digitalWrite(DRV1_B, preferences.getInt("DRV1_B_State", 0));
        if (bitRead(readGPIO, DRV1_B) == 0 and preferences.getInt("DRV1_B_State", 0) == 1)
          PwdPompKick(PwdChannel1, KickUpMax, KickUpStrart, pwd_val, KickUpTime);

        mcp.digitalWrite(DRV1_C, preferences.getInt("DRV1_C_State", 0));
        if (bitRead(readGPIO, DRV1_C) == 0 and preferences.getInt("DRV1_C_State", 0) == 1)
          PwdPompKick(PwdChannel2, KickUpMax, KickUpStrart, pwd_val, KickUpTime);

        mcp.digitalWrite(DRV1_D, preferences.getInt("DRV1_D_State", 0));
        if (bitRead(readGPIO, DRV1_D) == 0 and preferences.getInt("DRV1_D_State", 0) == 1)
          PwdPompKick(PwdChannel2, KickUpMax, KickUpStrart, pwd_val, KickUpTime);

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

        readGPIO = mcp.readGPIOAB();

        for (int p = 0; p < 16; p++)
        {
          int stat = bitRead(readGPIO, p);
          if (stat != 0)
            syslog_ng("MCP23017 port" + String(p) + ":" + String(stat));
        }

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