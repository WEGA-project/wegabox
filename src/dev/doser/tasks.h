#if c_MCP23017 == 1 and c_DOSER == 1
void TaskDOSER(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    delay(100);

    unsigned long DOSER_LastTime = millis() - DOSER_old;

    if (xSemaphoreX != NULL and DOSER_LastTime > DOSER_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        unsigned long DOSER_time = millis();
        syslog_ng("DOSER Start " + fFTS(DOSER_LastTime - DOSER_Repeat, 0) + "ms");

        AA = preferences.getInt("StPumpA_A", 4);
        AB = preferences.getInt("StPumpA_B", 5);
        AC = preferences.getInt("StPumpA_C", 6);
        AD = preferences.getInt("StPumpA_D", 7);

        BA = preferences.getInt("StPumpB_A", 8);
        BB = preferences.getInt("StPumpB_B", 9);
        BC = preferences.getInt("StPumpB_C", 10);
        BD = preferences.getInt("StPumpB_D", 11);

        mcp.pinMode(AA, OUTPUT);
        mcp.pinMode(AB, OUTPUT);
        mcp.pinMode(AC, OUTPUT);
        mcp.pinMode(AD, OUTPUT);

        mcp.pinMode(BA, OUTPUT);
        mcp.pinMode(BB, OUTPUT);
        mcp.pinMode(BC, OUTPUT);
        mcp.pinMode(BD, OUTPUT);

        // Налить 10 мл
        float Tgt = 10;
        StPumpA_cStepMl = preferences.getFloat("StPumpA_cStepMl", 1000);
        StPumpA_cMl = preferences.getFloat("StPumpA_cMl", 1);
        del = preferences.getInt("StPumpA_Del", 700);
        float CurrentMl = 0;
        while (CurrentMl <= Tgt)
        {
          if (OtaStart == true)
            vTaskDelete(NULL);

          StepTwoDrvForward(AA, AB, AC, AD, BA, BB, BC, BD, del);
          CurrentMl = CurrentMl + (StPumpA_cMl / StPumpA_cStepMl);
        }

        // for (long i = 0; i <= 40000; i++)
        // {
        // del=preferences.getInt("StPumpA_Del", 700);
        // StepTwoDrvForward(AA,AB,AC,AD,BA,BB,BC,BD,del);

        //   if (OtaStart == true) vTaskDelete(NULL);
        // }

        // for (long i = 0; i <= 40000; i++) // reverse
        // {
        //   del=preferences.getInt("StPumpA_Del", 700);
        //   StepTwoDrvBackward(AA,AB,AC,AD,BA,BB,BC,BD,del);
        //   if (OtaStart == true)
        //     vTaskDelete(NULL);
        // }

        mcp.pinMode(AA, LOW);
        mcp.pinMode(AB, LOW);
        mcp.pinMode(AC, LOW);
        mcp.pinMode(AD, LOW);

        mcp.pinMode(BA, LOW);
        mcp.pinMode(BB, LOW);
        mcp.pinMode(BC, LOW);
        mcp.pinMode(BD, LOW);
        syslog_ng("DOSER pHmV:" + fFTS(pHmV, 3));

        DOSER_time = millis() - DOSER_time;

        syslog_ng("DOSER " + fFTS(DOSER_time, 0) + "ms end.");
        DOSER_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}

#endif // c_DOSER