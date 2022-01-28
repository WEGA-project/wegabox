#if c_BMP280 == 1
void TaskBMP280(void *parameters)
{
  for (;;)
  { //
    if (OtaStart == true)
      vTaskDelete(NULL);
    //syslog_ng("BMP280 loop");  
    vTaskDelay(1000);

    unsigned long BMP280_LastTime = millis() - BMP280_old;

    if (xSemaphoreX != NULL and BMP280_LastTime > BMP280_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long BMP280_time = millis();
        syslog_ng("BMP280 Start " + fFTS(BMP280_LastTime - BMP280_Repeat, 0) + "ms");

          if (!bmx280.measure())
          {
            Serial.println("could not start measurement, is a measurement already running?");
            return;
          }

          //wait for the measurement to finish
          do
          {
            delay(100);
          } while (!bmx280.hasValue());

          AirTemp = bmx280.getTemperature();
          AirPress = bmx280.getPressure64() * 0.00750063755419211;

          if (bmx280.isBME280()) // Если датчик BME280 еще и влажность определить
          {
            AirHum = bmx280.getHumidity();
          }
        BMP280_time = millis() - BMP280_time;
        syslog_ng("BMP280 AirTemp:" + fFTS(AirTemp, 3));
        if (bmx280.isBME280()) syslog_ng("BMP280 AirHum:" + fFTS(AirHum, 3));
        syslog_ng("BMP280 AirPress:" + fFTS(AirPress, 3));        
        syslog_ng("BMP280 " + fFTS(BMP280_time, 0) + "ms end.");
        BMP280_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}

#endif //c_BMP280