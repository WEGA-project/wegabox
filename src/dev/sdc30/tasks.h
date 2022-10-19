#if c_SDC30 == 1
void TaskSDC30(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(1000);

    unsigned long SDC30_LastTime = millis() - SDC30_old;

    if (xSemaphoreX != NULL and SDC30_LastTime > SDC30_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long SDC30_time = millis();
        syslog_ng("SDC30 Start " + fFTS(SDC30_LastTime - SDC30_Repeat, 0) + "ms");

        if (airSensor.dataAvailable())
        {

          CO2 = airSensor.getCO2();
          AirTemp = airSensor.getTemperature();
          AirHum = airSensor.getHumidity();
          if (airSensor.getAutoSelfCalibration() == true)
            syslog_ng("SDC30 Warning: Enable Auto Self Calibration");
          syslog_ng("SDC30 CO2 (ppm): " + fFTS(CO2, 3));
          syslog_ng("SDC30 AirTemp: " + fFTS(AirTemp, 3));
          syslog_ng("SDC30 AirHum: " + fFTS(AirHum, 3));
        }
        else
        {
          syslog_ng("SDC30 Error - restart");
          airSensor.begin();
        }
        SDC30_time = millis() - SDC30_time;
        syslog_ng("SDC30 " + fFTS(SDC30_time, 0) + "ms end.");
        SDC30_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}

#endif // c_SDC30