#if c_VL6180X == 1

while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
  ;

Wire.requestFrom(VL6180Xaddr, (uint8_t)1);
if (Wire.available())
{

  s_vl6180X.init();
  s_vl6180X.configureDefault();
  s_vl6180X.setScaling(1);
  s_vl6180X.setTimeout(100);
  s_vl6180X.stopContinuous();
  s_vl6180X.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 20);
  s_vl6180X.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);


  xTaskCreate(TaskVL6180X, "VL6180X", 10000, NULL, 0, NULL);
  syslog_ng("VL6180X: add Task");
}
else
  syslog_err("VL6180X: The sensor is not detected");
xSemaphoreGive(xSemaphoreX);

#endif // c_VL6180X