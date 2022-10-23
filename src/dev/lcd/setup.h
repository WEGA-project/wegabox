#if c_LCD == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE)
    ;

Wire.requestFrom(LCDaddr, (uint8_t)1);
if (Wire.available())
{
    xTaskCreatePinnedToCore(TaskLCD, "TaskLCD", 8000, NULL, 0, NULL, 0);
    syslog_ng("LCD: add Task");
}
else
    syslog_err("LCD: not detected");
xSemaphoreGive(xSemaphoreX);
#endif // c_LCD
