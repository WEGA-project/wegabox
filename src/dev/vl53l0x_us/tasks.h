#if c_VL53L0X_us == 1
void TaskVL53L0X(void *parameters)
{

  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    delay(100);
    unsigned long VL53L0X_LastTime = millis() - VL53L0X_old;

    if (xSemaphoreX != NULL and VL53L0X_LastTime > VL53L0X_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        unsigned long VL53L0X_time = millis();
        syslog_ng("VL53L0X Start " + fFTS(VL53L0X_LastTime - VL53L0X_Repeat, 0) + "ms");
        Wire.end();
        delay(100);
        Wire.begin(US_SDA, US_SCL);

        delay(100);
        long err = 0;
        float range = 0;
        float range0 = 0;
        unsigned cont = 0;
        unsigned long t = millis();

        while (millis() - t < 5000)
        {
          // range0 = s_VL53L0X.readRangeSingleMillimeters();
          range0 = s_VL53L0X.readRangeContinuousMillimeters();
          if (s_VL53L0X.timeoutOccurred())
          {
            syslog_ng("VL53L0X: TIMEOUT");
          }
          if (range0 < 8000)
          {
            VL53L0X_RangeRM.add(range0);
            cont++;
          }
          else
            err++;
        }

        range = VL53L0X_RangeRM.getMedian() / 10;
        VL53L0X_RangeAVG.add(range);

        Dist = VL53L0X_RangeAVG.getAverage();
        if (abs(Dist - range) > 4)
        {
          VL53L0X_RangeAVG.clear();
          syslog_ng("VL53L0X: Reset average filter");
        }

        VL53L0X_time = millis() - VL53L0X_time;
        syslog_ng("VL53L0X: dist=" + fFTS(Dist, 3));
        syslog_ng("VL53L0X: Error/Count " + String(err) + "/" + String(cont));
        syslog_ng("VL53L0X: Highest= " + fFTS(VL53L0X_RangeRM.getHighest(), 1));
        syslog_ng("VL53L0X: Lowest= " + fFTS(VL53L0X_RangeRM.getLowest(), 1));

        syslog_ng("VL53L0X " + fFTS(VL53L0X_time, 0) + "ms end.");

        Wire.end();
        delay(100);
        Wire.begin(I2C_SDA, I2C_SCL);
        VL53L0X_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif // c_VL53L0X_us