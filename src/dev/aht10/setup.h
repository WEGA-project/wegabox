#if c_AHT10 == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
  ;

Wire.requestFrom(AHT10addr, (uint8_t)1);
if (Wire.available())
{

  myAHT10.softReset();
  delay(50);
  myAHT10.begin();
  myAHT10.setNormalMode();

  xTaskCreate(TaskAHT10, "TaskAHT10", 5000, NULL, 1, NULL);
  syslog_ng("AHT10: add Task");
}
else
  syslog_err("AHT10: The sensor is not detected");
xSemaphoreGive(xSemaphoreX);
#endif // c_TaskAHT10
