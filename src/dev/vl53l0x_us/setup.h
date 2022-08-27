#if c_VL53L0X_us == 1

Wire.begin(US_SDA, US_SCL);

s_VL53L0X.init();
s_VL53L0X.setTimeout(500);
s_VL53L0X.setSignalRateLimit(0.01);

s_VL53L0X.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
s_VL53L0X.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

s_VL53L0X.setMeasurementTimingBudget(200000);

Wire.begin(I2C_SDA, I2C_SCL);


xTaskCreate(TaskVL53L0X, "VL53L0X", 10000, NULL, 0, NULL);
syslog_ng("VL53L0X add Task");

#endif // c_VL53L0X_us