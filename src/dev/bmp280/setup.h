#if c_BMP280 == 1

while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
  ;

Wire.requestFrom(0x38, (uint8_t)1);
if (Wire.available())
{
  AHTx = true;
  syslog_ng("BMx280: found sensor AHTx! Hum and Temp disable");
}

Wire.requestFrom(0x76, (uint8_t)1);
if (Wire.available()){
  BMP280addr = 0x76;
  syslog_ng("BMx280: found on 0x76 address");
  }

Wire.requestFrom(0x77, (uint8_t)1);
if (Wire.available()){
  BMP280addr = 0x77;
  syslog_ng("BMx280: found on 0x77 address");
  }

Wire.requestFrom(BMP280addr, (uint8_t)1);
if (Wire.available())
{

  bmx280.begin();

  if (bmx280.isBME280())
    syslog_ng("BMx280: found sensor is a BME280");
  else
    syslog_ng("BMx280: found sensor is a BMP280");

  // reset sensor to default parameters.
  bmx280.resetToDefaults();

  // by default sensing is disabled and must be enabled by setting a non-zero
  // oversampling setting.
  // set an oversampling setting for pressure and temperature measurements.
  bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
  bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

  // if sensor is a BME280, set an oversampling setting for humidity measurements.
  if (bmx280.isBME280())
    bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);

  xTaskCreate(TaskBMP280, "TaskBMP280", 10000, NULL, 0, NULL);
  syslog_ng("BMx280: add Task");
}
else
  syslog_err("BMx280: The sensor is not detected");

xSemaphoreGive(xSemaphoreX);
#endif // c_BME280
