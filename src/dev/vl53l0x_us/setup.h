#if c_VL53L0X_us == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
  ;

Wire.begin(US_SDA, US_SCL);

Wire.requestFrom(0x29, (uint8_t)1);
if (Wire.available())
{
  syslog_ng("I2C-US found: 0x29 laser distance sensor");
  if (s_VL53L0X.init())
  {
    syslog_ng("VL53L0X: sensor detected");
    s_VL53L0X.setTimeout(500);
    s_VL53L0X.setSignalRateLimit(0.01);

    s_VL53L0X.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    s_VL53L0X.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

    s_VL53L0X.setMeasurementTimingBudget(200000);

    xTaskCreate(TaskVL53L0X, "VL53L0X", 10000, NULL, 0, NULL);
    syslog_ng("VL53L0X: add Task");
  }
  else
    syslog_err("VL53L0X: The sensor is not detected");
}

Wire.begin(I2C_SDA, I2C_SCL);
xSemaphoreGive(xSemaphoreX);
#endif // c_VL53L0X_us