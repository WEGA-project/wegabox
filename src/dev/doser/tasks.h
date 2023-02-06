#if c_MCP23017 == 1 and c_DOSER == 1
void TaskDOSER(void *parameters) {
  for (;;) {
    if (OtaStart == true)
      vTaskDelete(NULL);
    delay(100);

    unsigned long DOSER_LastTime = millis() - DOSER_old;

    if (xSemaphoreX != NULL and DOSER_LastTime > DOSER_Repeat) {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)3) == pdTRUE) {
        unsigned long DOSER_time = millis();
        syslog_ng("DOSER Start " + fFTS(DOSER_LastTime - DOSER_Repeat, 0) +
                  "ms");

        SetPumpA_Ml = preferences.getFloat("SetPumpA_Ml", 0);
        SetPumpB_Ml = preferences.getFloat("SetPumpB_Ml", 0);

        if (SetPumpA_Ml > 0 or SetPumpB_Ml > 0) {
          syslog_ng("DOSER: PumpA remains ml=" + fFTS(SetPumpA_Ml, 2));
          syslog_ng("DOSER: PumpB remains ml=" + fFTS(SetPumpB_Ml, 2));

          AA = preferences.getInt("StPumpA_A", 4);
          AB = preferences.getInt("StPumpA_B", 5);
          AC = preferences.getInt("StPumpA_C", 6);
          AD = preferences.getInt("StPumpA_D", 7);
          ADel = preferences.getInt("StPumpA_Del", 700);
          ARet = preferences.getInt("StPumpA_Ret", 700);
          mcp.pinMode(AA, OUTPUT);
          mcp.pinMode(AB, OUTPUT);
          mcp.pinMode(AC, OUTPUT);
          mcp.pinMode(AD, OUTPUT);

          BA = preferences.getInt("StPumpB_A", 8);
          BB = preferences.getInt("StPumpB_B", 9);
          BC = preferences.getInt("StPumpB_C", 10);
          BD = preferences.getInt("StPumpB_D", 11);
          BDel = preferences.getInt("StPumpB_Del", 700);
          BRet = preferences.getInt("StPumpB_Ret", 700);
          mcp.pinMode(BA, OUTPUT);
          mcp.pinMode(BB, OUTPUT);
          mcp.pinMode(BC, OUTPUT);
          mcp.pinMode(BD, OUTPUT);

          StPumpA_cStepMl = preferences.getFloat("StPumpA_cStepMl", 500);
          StPumpA_cMl = preferences.getFloat("StPumpA_cMl", 1);

          AOn = preferences.getInt("StPumpA_On", 0);
          StPumpA_cStep = preferences.getFloat("StPumpA_cStep", 500);
          float ALeftStep = (SetPumpA_Ml / StPumpA_cMl) * StPumpA_cStepMl;
          if (ALeftStep < StPumpA_cStep)
            StPumpA_cStep = ALeftStep; // Если до конца цикла осталось меньше

          if (SetPumpA_Ml > 0 and AOn != 0) {
            syslog_ng("DOSER: PumpA Start");
            // Облегчить старт коротким реверсом
            for (long i = 0; i < 5; i++)
              StepAB(1, 1, 1);
            for (long i = 0; i < 5; i++)
              StepAF(1, 1, 1);

            for (long i = 0; i < StPumpA_cStep; i++) {
              StepAF(true, true, true);
              if (OtaStart == true) {
                // mcp.writeGPIOAB(0);
                bitW4(AA, AB, AC, AD, 0, 0, 0, 0);
                bitW4(BA, BB, BC, BD, 0, 0, 0, 0);
                mcp.writeGPIOAB(bitw);
                preferences.putFloat("SetPumpA_Ml", SetPumpA_Ml - (StPumpA_cMl / StPumpA_cStepMl * i));
                preferences.putFloat("SetPumpA_Ml_SUM", preferences.getFloat("SetPumpA_Ml_SUM", 0) + (StPumpA_cMl / StPumpA_cStepMl * i));
                preferences.putLong("PumpA_Step_SUM", preferences.getLong("PumpA_Step_SUM",0) + i);
                vTaskDelete(NULL);
              }
            }
            preferences.putFloat("SetPumpA_Ml", SetPumpA_Ml - (StPumpA_cMl / StPumpA_cStepMl * StPumpA_cStep));

            preferences.putFloat("SetPumpA_Ml_SUM", preferences.getFloat("SetPumpA_Ml_SUM", 0) + (StPumpA_cMl / StPumpA_cStepMl * StPumpA_cStep));
            preferences.putLong("PumpA_Step_SUM", preferences.getLong("PumpA_Step_SUM",0) + StPumpA_cStep);
            syslog_ng("DOSER: PumpA Stop");
          }

          StPumpB_cStepMl = preferences.getFloat("StPumpB_cStepMl", 500);
          StPumpB_cMl = preferences.getFloat("StPumpB_cMl", 1);
          BOn = preferences.getInt("StPumpB_On", 0);
          StPumpB_cStep = preferences.getFloat("StPumpB_cStep", 500);
          float BLeftStep = (SetPumpB_Ml / StPumpB_cMl) * StPumpB_cStepMl;
          if (BLeftStep < StPumpB_cStep)
            StPumpB_cStep = BLeftStep; // Если до конца цикла осталось меньше

          if (SetPumpB_Ml > 0 and BOn != 0) {
            syslog_ng("DOSER: PumpB Start");
                        for (long i = 0; i < 5; i++)
              StepBB(1, 1, 1);
            for (long i = 0; i < 5; i++)
              StepBF(1, 1, 1);

            for (long i = 0; i < StPumpB_cStep; i++) {
              StepBF(true, true, true);

              if (OtaStart == true) {
                // mcp.writeGPIOAB(0);
                bitW4(AA, AB, AC, AD, 0, 0, 0, 0);
                bitW4(BA, BB, BC, BD, 0, 0, 0, 0);
                mcp.writeGPIOAB(bitw);
                preferences.putFloat("SetPumpB_Ml",SetPumpB_Ml - (StPumpB_cMl / StPumpB_cStepMl * i));

                preferences.putFloat("SetPumpB_Ml_SUM", preferences.getFloat("SetPumpB_Ml_SUM", 0) + (StPumpB_cMl / StPumpB_cStepMl * i));
                preferences.putLong("PumpB_Step_SUM", preferences.getLong("PumpB_Step_SUM",0) + i);
                vTaskDelete(NULL);
              }
            }
            preferences.putFloat("SetPumpB_Ml",SetPumpB_Ml - (StPumpB_cMl / StPumpB_cStepMl * StPumpB_cStep));

            preferences.putFloat("SetPumpB_Ml_SUM", preferences.getFloat("SetPumpB_Ml_SUM", 0) + (StPumpB_cMl / StPumpB_cStepMl * StPumpB_cStep));
            preferences.putLong("PumpB_Step_SUM", preferences.getLong("PumpB_Step_SUM",0) + StPumpB_cStep);
            syslog_ng("DOSER: PumpB Stop");
          }

          // mcp.pinMode(AA, LOW);
          // mcp.pinMode(AB, LOW);
          // mcp.pinMode(AC, LOW);
          // mcp.pinMode(AD, LOW);
          // mcp.pinMode(BA, LOW);
          // mcp.pinMode(BB, LOW);
          // mcp.pinMode(BC, LOW);
          // mcp.pinMode(BD, LOW);

          bitW4(AA, AB, AC, AD, 0, 0, 0, 0);
          bitW4(BA, BB, BC, BD, 0, 0, 0, 0);
          mcp.writeGPIOAB(bitw);

          syslog_ng("DOSER: PumpA SUM ml=" + fFTS(preferences.getFloat("SetPumpA_Ml_SUM", 0), 2));
          syslog_ng("DOSER: PumpB SUM ml=" + fFTS(preferences.getFloat("SetPumpB_Ml_SUM", 0), 2));
        } else
          syslog_ng("DOSER: Nothing to do");

        DOSER_time = millis() - DOSER_time;

        syslog_ng("DOSER " + fFTS(DOSER_time, 0) + "ms end.");
        DOSER_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}

#endif // c_DOSER