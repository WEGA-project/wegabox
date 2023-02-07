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

        syslog_ng("VL6180X Range:" + fFTS(s_vl6180X.readRangeContinuousMillimeters(), 0));


        s_vl6180X.setScaling(vl6180XScalling);
        //int32_t rangemm = s_vl6180X.readRangeSingleMillimeters();
        int32_t rangemm = s_vl6180X.readRangeContinuousMillimeters();

        if (rangemm < 250)
          vl6180XScalling = 1;
        if (rangemm >= 250 and rangemm < 500)
          vl6180XScalling = 2;
        if (rangemm >= 500)
          vl6180XScalling = 3;

        s_vl6180X.setScaling(vl6180XScalling);

        delay(100);
        long err = 0;
        float range0 = 0;
        unsigned cont = 0;
        unsigned long t = millis();
        while (millis() - t < 5000)
        {
          //s_vl6180X.timeoutOccurred();
          range0 = s_vl6180X.readRangeContinuousMillimeters();
          if (range0 != 768)
          {
            VL6180X_RangeRM.add(range0);
            cont++;
          }
          else
          {
            syslog_err("VL6180X: Error range");
            err++;
            Wire.begin();
          }
        }



        VL6180X_RangeAVG.add(VL6180X_RangeRM.getMedian() / 10);
        Dist=VL6180X_RangeAVG.getAverage();

        VL6180X_time = millis() - VL6180X_time;
         syslog_ng("VL6180X: dist=" + fFTS(Dist, 3));
        syslog_ng("VL6180X: range=" + String(rangemm)+" mm");
        syslog_ng("VL6180X: Scalling: " + String(vl6180XScalling));
        syslog_ng("VL6180X: Error/Count " + String(err) + "/" + String(cont));
        syslog_ng("VL6180X: Highest= " + fFTS(VL6180X_RangeRM.getHighest(), 1));
        syslog_ng("VL6180X: Lowest= " + fFTS(VL6180X_RangeRM.getLowest(), 1));

        syslog_ng("VL6180X " + fFTS(VL6180X_time, 0) + "ms end.");
        VL6180X_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif // c_VL6180X