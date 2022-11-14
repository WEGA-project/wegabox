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

        mcp.pinMode(A, OUTPUT);
        mcp.pinMode(B, OUTPUT);
        mcp.pinMode(C, OUTPUT);
        mcp.pinMode(D, OUTPUT);

        for (int i = 0; i <= 1000; i++)
        {

          // enn
          bitWrite(bitw, A, 0);
          bitWrite(bitw, B, 1);
          bitWrite(bitw, C, 1);
          bitWrite(bitw, D, 0);
          mcp.writeGPIOAB(bitw);
          delayMicroseconds(del);

          // twee
          bitWrite(bitw, A, 0);
          bitWrite(bitw, B, 1);
          bitWrite(bitw, C, 0);
          bitWrite(bitw, D, 1);
          mcp.writeGPIOAB(bitw);
          delayMicroseconds(del);

          // drie
          bitWrite(bitw, A, 1);
          bitWrite(bitw, B, 0);
          bitWrite(bitw, C, 0);
          bitWrite(bitw, D, 1);
          mcp.writeGPIOAB(bitw);
          delayMicroseconds(del);

          // vier
          bitWrite(bitw, A, 1);
          bitWrite(bitw, B, 0);
          bitWrite(bitw, C, 1);
          bitWrite(bitw, D, 0);
          mcp.writeGPIOAB(bitw);
          delayMicroseconds(del);

          bitWrite(bitw, A, 0);
          bitWrite(bitw, B, 0);
          bitWrite(bitw, C, 0);
          bitWrite(bitw, D, 0);
          mcp.writeGPIOAB(bitw);
          delayMicroseconds(del);

          if (OtaStart == true)
            vTaskDelete(NULL);
        }

        

        for (int i = 0; i <= 1000; i++) // reverse
        {

          // enn
          bitWrite(bitw, A, 0);
          bitWrite(bitw, B, 1);
          bitWrite(bitw, C, 1);
          bitWrite(bitw, D, 0);
          mcp.writeGPIOAB(bitw);
          delayMicroseconds(del);

          // twee
          bitWrite(bitw, A, 1);
          bitWrite(bitw, B, 0);
          bitWrite(bitw, C, 1);
          bitWrite(bitw, D, 0);
          mcp.writeGPIOAB(bitw);
          delayMicroseconds(del);

          // drie
          bitWrite(bitw, A, 1);
          bitWrite(bitw, B, 0);
          bitWrite(bitw, C, 0);
          bitWrite(bitw, D, 1);
          mcp.writeGPIOAB(bitw);
          delayMicroseconds(del);

          // vier
          bitWrite(bitw, A, 0);
          bitWrite(bitw, B, 1);
          bitWrite(bitw, C, 0);
          bitWrite(bitw, D, 1);
          mcp.writeGPIOAB(bitw);
          delayMicroseconds(del);

          bitWrite(bitw, A, 0);
          bitWrite(bitw, B, 0);
          bitWrite(bitw, C, 0);
          bitWrite(bitw, D, 0);
          mcp.writeGPIOAB(bitw);
          delayMicroseconds(del);

          if (OtaStart == true)
            vTaskDelete(NULL);
        }

        mcp.pinMode(A, LOW);
        mcp.pinMode(B, LOW);
        mcp.pinMode(C, LOW);
        mcp.pinMode(D, LOW);

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