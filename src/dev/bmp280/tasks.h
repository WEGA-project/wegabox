#if c_BMP280 == 1
void TaskBMP280(void *parameters)
{

while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE);

// Autodetect address for BMx280 0x76 or 0x77

Wire.requestFrom(0x76, (uint8_t)1);
if (Wire.available()){
  BMP280addr = 0x76;
  syslog_ng("BMx280: found on 0x76 address");
  }

Wire.requestFrom(0x77, (uint8_t)1);
if (Wire.available()){
  BMP280addr = 0x77;
  syslog_ng("BMx280: found on 0x77 address");
  }

BMx280I2C bmx280(BMP280addr);


  bmx280.begin();

  if (bmx280.isBME280())
    syslog_ng("BMx280: found sensor is a BME280");
  else
    syslog_ng("BMx280: found sensor is a BMP280");

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
xSemaphoreGive(xSemaphoreX);


  for (;;)
  { //
    if (OtaStart == true)
      vTaskDelete(NULL);
    // syslog_ng("BMP280 loop");
    vTaskDelay(100);

    unsigned long BMP280_LastTime = millis() - BMP280_old;

    if (xSemaphoreX != NULL and BMP280_LastTime > BMP280_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        unsigned long BMP280_time = millis();
        syslog_ng("BMx280 Start " + fFTS(BMP280_LastTime - BMP280_Repeat, 0) + "ms");

        if (!bmx280.measure())
        {
          syslog_ng("BMx280: could not start measurement, is a measurement already running?");
          return;
        }

        // wait for the measurement to finish
        do
        {
          delay(100);
        } while (!bmx280.hasValue());
        
        BMP280_Press = bmx280.getPressure64() * 0.00750063755419211;
        syslog_ng("BMx280 AirPress:" + fFTS(BMP280_Press, 3));

          if (AHTx == false)
          {
            BMP280_Temp = bmx280.getTemperature();
            BMP280_AirTempRM.add(BMP280_Temp);
            AirTemp = BMP280_AirTempRM.getAverage(10);
            syslog_ng("BMx280 AirTemp:" + fFTS(AirTemp, 3));

            if (bmx280.isBME280())
            {
              BMP280_AirHumRM.add(bmx280.getHumidity());
              AirHum = BMP280_AirHumRM.getAverage(10);
              syslog_ng("BMx280 AirHum:" + fFTS(AirHum, 3));
            }
          }

          BMP280_AirPressRM.add(BMP280_Press);
          AirPress = BMP280_AirPressRM.getAverage(10);


        BMP280_time = millis() - BMP280_time;

 
          


        syslog_ng("BMx280 " + fFTS(BMP280_time, 0) + "ms end.");
        BMP280_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}

#endif // c_BMP280