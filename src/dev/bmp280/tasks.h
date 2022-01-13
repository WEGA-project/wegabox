#if c_BMP280 == 1
  void TaskBMP280(void *parameters)
  {
    for (;;)
    {
      if (OtaStart == true)
      {
        vTaskDelete(NULL);
      }
      else
      {

        if (xSemaphoreTake(xI2CSemaphore, (TickType_t)5) == pdTRUE)
        {

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

          xSemaphoreGive(xI2CSemaphore);
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      }
    }
  }
#endif //c_BMP280