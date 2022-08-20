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

        // Serial.print("Ambient: ");
        // syslog_ng("VL6180X Ambient:" + fFTS(s_vl6180X.readAmbientSingle(),0 ) );
        //  if (s_vl6180X.timeoutOccurred()) { syslog_err("VL6180X Ambient: TIMEOUT"); }

        // //Serial.print("\tRange: ");
        Wire.begin(13,14);


        syslog_ng("VL53L0X Range:" + fFTS(s_VL53L0X.readRangeSingleMillimeters(), 0));

        delay(100);
        long err = 0;
        float range = 0;
        float range0 = 0;
        unsigned cont = 0;
        unsigned long t = millis();
        while (millis() - t < 1000)
        {
          cont++;

          if (s_VL53L0X.timeoutOccurred())
          syslog_err("VL53L0X dist: TIMEOUT");

          range0 = s_VL53L0X.readRangeSingleMillimeters();
          if (range0 != 765)
            range = range0 / 10 + range;
        }
        VL53L0X_RangeRM.add(range / (cont - err));
        Dist = VL53L0X_RangeRM.getAverage(100);

        VL53L0X_time = millis() - VL53L0X_time;

        syslog_ng("VL53L0X dist: " + fFTS(Dist, 3));
        syslog_ng("VL53L0X Error/Count: " + String(err) + "/" + String(cont));
        syslog_ng("VL53L0X Highest: " + fFTS(VL53L0X_RangeRM.getHighest(), 1));
        syslog_ng("VL53L0X Lowest: " + fFTS(VL53L0X_RangeRM.getLowest(), 1));

        syslog_ng("VL53L0X " + fFTS(VL53L0X_time, 0) + "ms end.");

        Wire.begin(I2C_SDA, I2C_SCL);
        VL53L0X_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif // c_VL53L0X_us