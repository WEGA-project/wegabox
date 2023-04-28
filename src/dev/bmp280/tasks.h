#if c_BMP280 == 1
void TaskBMx280(void *parameters)
{






BMx280I2C bmx280(BMx280addr);
  bmx280.begin();

  // reset sensor to default parameters.
  bmx280.resetToDefaults();

  // by default sensing is disabled and must be enabled by setting a non-zero
  // oversampling setting.
  // set an oversampling setting for pressure and temperature measurements.
  bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
  bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

  // if sensor is a BME280, set an oversampling setting for humidity measurements.
  if (bmx280.isBME280())
    bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);



  for (;;)
  { //
    if (OtaStart == true)
      vTaskDelete(NULL);
    // syslog_ng("BMP280 loop");
    vTaskDelay(100);

    unsigned long BMP280_LastTime = millis() - BMx280_old;

    if (xSemaphoreX != NULL and BMP280_LastTime > BMx280_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        unsigned long BMP280_time = millis();
        syslog_ng("BMx280 Start " + fFTS(BMP280_LastTime - BMx280_Repeat, 0) + "ms");

        if (!bmx280.measure())
        {
          syslog_ng("BMx280: could not start measurement, is a measurement already running?");
          return;
        }

        // wait for the measurement to finish

        for (int i=0;!bmx280.hasValue() and i<10;i++){
          delay(100);
          if (i>9) syslog_err("BMx280: wait end for the measurement");
        }
        // do
        // {
        //   delay(100);
        // } while (!bmx280.hasValue());
        
        BMx280_Press = bmx280.getPressure64() * 0.00750063755419211;
        syslog_ng("BMx280 AirPress:" + fFTS(BMx280_Press, 3));

          if (AHTx == false)
          {
            BMx280_Temp = bmx280.getTemperature();
            BMx280_AirTempRM.add(BMx280_Temp);
            AirTemp = BMx280_AirTempRM.getAverage(10);
            syslog_ng("BMx280 AirTemp:" + fFTS(AirTemp, 3));

            if (bmx280.isBME280())
            {
              BMx280_AirHumRM.add(bmx280.getHumidity());
              AirHum = BMx280_AirHumRM.getAverage(10);
              syslog_ng("BMx280 AirHum:" + fFTS(AirHum, 3));
            }
          }

          BMx280_AirPressRM.add(BMx280_Press);
          AirPress = BMx280_AirPressRM.getAverage(10);


        BMP280_time = millis() - BMP280_time;

 
          


        syslog_ng("BMx280 " + fFTS(BMP280_time, 0) + "ms end.");
        BMx280_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}

#endif // c_BMP280