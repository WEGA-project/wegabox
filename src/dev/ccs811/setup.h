#if c_CCS811 == 1
// Enable CCS811
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
  ;

Wire.requestFrom(CCS811addr, (uint8_t)1);
if (Wire.available())
{


ccs811.set_i2cdelay(50);
bool ok = ccs811.begin();

// Check if flashing should be executed
if (ccs811.application_version() == 0x2000)
{
  syslog_ng("CCS811: init... already has 2.0.0");
}
else
{

  delay(5000);
  ok = ccs811.flash(image_data, sizeof(image_data));
  if (!ok)
    syslog_ng("CCS811: flash FAILED");

}
ccs811.start(CCS811_MODE_1SEC);


xTaskCreate(TaskCCS811, "TaskCCS811", 10000, NULL, 0, &appTasks[appTaskCount++]);
  syslog_ng("CCS811: add Task");
}
else
  syslog_err("CCS811: The sensor is not detected");

xSemaphoreGive(xSemaphoreX);
#endif // c_CCS811