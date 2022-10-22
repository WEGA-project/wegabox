#if c_SDC30 == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
  ;

Wire.requestFrom(SDC30addr, (uint8_t)1);
if (Wire.available())
{

  if (airSensor.begin() == false) // Pass the Wire port to the .begin() function
  {
    syslog_ng("SDC30: Air sensor not detected.");
  }

  xTaskCreate(TaskSDC30, "TaskSDC30", 8000, NULL, 0, NULL);

  syslog_ng("SDC30: add Task");
}
else
  syslog_err("SDC30: not detected");
xSemaphoreGive(xSemaphoreX);
#endif // c_SDC30