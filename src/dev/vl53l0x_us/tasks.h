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

        Wire.begin(US_SDA, US_SCL);

s_VL53L0X.init();
s_VL53L0X.setTimeout(500);
s_VL53L0X.setSignalRateLimit(0.01);

s_VL53L0X.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
s_VL53L0X.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

s_VL53L0X.setMeasurementTimingBudget(200000);




        long range = s_VL53L0X.readRangeSingleMillimeters();
        if (range != 65535)
          VL53L0X_RangeRM.add(range);
        else
          VL53L0X_err++;
        Dist = float(VL53L0X_RangeRM.getAverage(50)) / 10;

        VL53L0X_time = millis() - VL53L0X_time;
        if (VL53L0X_err > 0)
          syslog_err("VL53L0X error count: " + String(VL53L0X_err));

        syslog_ng("VL53L0X range: " + String(range));
        syslog_ng("VL53L0X dist: " + fFTS(Dist, 3));
        syslog_ng("VL53L0X " + fFTS(VL53L0X_time, 0) + "ms end.");

        Wire.begin(I2C_SDA, I2C_SCL);
        VL53L0X_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif // c_VL53L0X_us