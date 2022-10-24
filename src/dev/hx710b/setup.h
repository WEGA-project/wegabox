#if c_HX710B == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
  ;

uint32_t HX710B_probe = -1;
if (HX710B_Press.init())
  HX710B_Press.read(&HX710B_probe, 1000UL);

if (HX710B_probe != -1)
{
  xTaskCreate(TaskHX710B, "TaskHX710B", 15000, NULL, 0, NULL);
  syslog_ng("HX710B: add Task");
}
else
  syslog_err("HX710B: The sensor is not detected");
xSemaphoreGive(xSemaphoreX);

#endif // c_HX710B
