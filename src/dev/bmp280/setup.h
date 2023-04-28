#if c_BMP280 == 1
BMx280addr = 0x76;

Wire.requestFrom((uint8_t)0x76, (uint8_t)1);
if (Wire.available())
{
  syslog_ng("BMx280: 0x76 add Task");
  BMx280addr = 0x76;
  xTaskCreate(TaskBMx280, "TaskBMx280", 10000, NULL, 0, NULL);
}
else
{
  Wire.requestFrom((uint8_t)0x77, (uint8_t)1);
  if (Wire.available())
  {
    syslog_ng("BMx280: 0x77 add Task");
    BMx280addr = 0x77;
    xTaskCreate(TaskBMx280, "TaskBMx280", 10000, NULL, 0, NULL);
  }
  else
    syslog_err("BMx280: The sensor is not detected");
}
#endif // c_BMx280
