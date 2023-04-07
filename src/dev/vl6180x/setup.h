#if c_VL6180X == 1

while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
  ;

Wire.requestFrom(VL6180Xaddr, (uint8_t)1);
if (Wire.available())
{



  xTaskCreate(TaskVL6180X, "VL6180X", 10000, NULL, 0, NULL);
  syslog_ng("VL6180X: add Task");
}
else
  syslog_err("VL6180X: The sensor is not detected");
xSemaphoreGive(xSemaphoreX);

#endif // c_VL6180X