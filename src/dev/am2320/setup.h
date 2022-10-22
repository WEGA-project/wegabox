#if c_AM2320 == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
  ;
Wire.requestFrom(AM2320addr, (uint8_t)1);
if (Wire.available())
{

  if (!AM2320.begin())
  {
    Serial.println("AM2320 Sensor not found");
  }

  xTaskCreate(TaskAM2320, "AM2320", 10000, NULL, 0, &appTasks[appTaskCount++]);

  syslog_ng("AM2320: add Task");
}
else
  syslog_err("AM2320: The sensor is not detected");
xSemaphoreGive(xSemaphoreX);
#endif // c_AM2320