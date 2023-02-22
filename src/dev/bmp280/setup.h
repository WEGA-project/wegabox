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
if (Wire.available())
{
  xTaskCreate(TaskBMP280, "TaskBMP280", 10000, NULL, 0, NULL);
  syslog_ng("BMx280 addr 0x76: add Task");
}
else
{
  Wire.requestFrom(0x77, (uint8_t)1);
  if (Wire.available())
  {
    xTaskCreate(TaskBMP280, "TaskBMP280", 10000, NULL, 0, NULL);
    syslog_ng("BMx280 addr 0x77: add Task");
  }
  else
    syslog_err("BMx280: The sensor is not detected");
}

xSemaphoreGive(xSemaphoreX);
#endif // c_BME280
