#if c_DualBMx == 1
    if (!bmx280_1.begin())
      {
        syslog_ng("Check your BMx280_1 Interface and I2C Address.");
      }

    if (!bmx280_2.begin())
      {
        syslog_ng("Check your BMx280_2 Interface and I2C Address.");
      }

      bmx280_1.resetToDefaults();
      bmx280_1.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
      bmx280_1.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

      bmx280_2.resetToDefaults();
      bmx280_2.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
      bmx280_2.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);


      //by default sensing is disabled and must be enabled by setting a non-zero
      //oversampling setting.
      //set an oversampling setting for pressure and temperature measurements. 


      //if sensor is a BME280, set an oversampling setting for humidity measurements.
      // if (bmx280.isBME280())
      //   bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);

xTaskCreate(TaskDualBMx,"TaskDualBMx",20000,NULL,0,NULL);
syslog_ng("DualBMx add Task");

#endif // c_DualBMx
