#if c_BMP280 == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE);

    if (!bmx280.begin())
      {
        Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
        while (1);
      }

      if (bmx280.isBME280())
        Serial.println("sensor is a BME280");
      else
        Serial.println("sensor is a BMP280");

      //reset sensor to default parameters.
      bmx280.resetToDefaults();

      //by default sensing is disabled and must be enabled by setting a non-zero
      //oversampling setting.
      //set an oversampling setting for pressure and temperature measurements. 
      bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
      bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

      //if sensor is a BME280, set an oversampling setting for humidity measurements.
      if (bmx280.isBME280())
        bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);

xSemaphoreGive(xSemaphoreX);


xTaskCreate(TaskBMP280,"TaskBMP280",10000,NULL,0,NULL);
syslog_ng("BMP280 add Task");

#endif // c_BME280
