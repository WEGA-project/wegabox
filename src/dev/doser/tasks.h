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

        for (int i = 0; i <= 500; i++)
        {
          // enn
          mcp.digitalWrite(A, LOW);
          mcp.digitalWrite(B, HIGH);
          mcp.digitalWrite(C, HIGH);
          mcp.digitalWrite(D, LOW);
          delayMicroseconds(del);

          // twee
          mcp.digitalWrite(A, LOW);
          mcp.digitalWrite(B, HIGH);
          mcp.digitalWrite(C, LOW);
          mcp.digitalWrite(D, HIGH);
          delayMicroseconds(del);

          // drie
          mcp.digitalWrite(A, HIGH);
          mcp.digitalWrite(B, LOW);
          mcp.digitalWrite(C, LOW);
          mcp.digitalWrite(D, HIGH);
          delayMicroseconds(del);

          // vier
          mcp.digitalWrite(A, HIGH);
          mcp.digitalWrite(B, LOW);
          mcp.digitalWrite(C, HIGH);
          mcp.digitalWrite(D, LOW);
          delayMicroseconds(del);

          lastOffDoser = millis();
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