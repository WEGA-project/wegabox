#if c_VL6180X == 1
void TaskVL6180X(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    delay(100);
    unsigned long VL6180X_LastTime = millis() - VL6180X_old;

    if (xSemaphoreX != NULL and VL6180X_LastTime > VL6180X_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        unsigned long VL6180X_time = millis();
        syslog_ng("VL6180X Start " + fFTS(VL6180X_LastTime - VL6180X_Repeat, 0) + "ms");

        // Serial.print("Ambient: ");
        // syslog_ng("VL6180X Ambient:" + fFTS(s_vl6180X.readAmbientSingle(),0 ) );
        //  if (s_vl6180X.timeoutOccurred()) { syslog_err("VL6180X Ambient: TIMEOUT"); }

        // //Serial.print("\tRange: ");
        syslog_ng("VL6180X Range:" + fFTS(s_vl6180X.readRangeSingleMillimeters(), 0));

        // syslog_ng("VL6180X RangeRAW:" + fFTS(s_vl6180X.readRangeSingle(),0 ) );
        //  if (s_vl6180X.timeoutOccurred()) { syslog_err("VL6180X Range: TIMEOUT"); }

        // syslog_ng("VL6180X Range:" + fFTS(s_vl6180X.readRangeContinuousMillimeters(),0 ) );
        s_vl6180X.init();
        s_vl6180X.setTimeout(100);

        delay(100);
        long err = 0;
        float range = 0;
        float range0 = 0;
        unsigned cont = 0;
        unsigned long t = millis();
        while (millis() - t < 1000)
        {
          cont++;

          s_vl6180X.timeoutOccurred();
          range0 = s_vl6180X.readRangeSingleMillimeters();
          if (range0 != 765)
            range = range0 / 10 + range;
          else
            err++;
          s_vl6180X.init();
          s_vl6180X.setTimeout(100);
        }
        VL6180X_RangeRM.add(range / (cont - err));
        Dist = VL6180X_RangeRM.getAverage(100);

        VL6180X_time = millis() - VL6180X_time;

        syslog_ng("VL6180X dist: " + fFTS(Dist, 3));
        syslog_ng("VL6180X Error/Count: " + String(err) + "/" + String(cont));
        syslog_ng("VL6180X Highest: " + fFTS(VL6180X_RangeRM.getHighest(), 1));
        syslog_ng("VL6180X Lowest: " + fFTS(VL6180X_RangeRM.getLowest(), 1));

        syslog_ng("VL6180X " + fFTS(VL6180X_time, 0) + "ms end.");
        VL6180X_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif // c_VL6180X