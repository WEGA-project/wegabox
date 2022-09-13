#if c_VL6180X_us == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE);


  Wire.begin(13,14);

  //I2Ctwo.begin(13, 14, 100000);
  s_vl6180X.init();
  s_vl6180X.configureDefault();

  // Reduce range max convergence time and ALS integration
  // time to 30 ms and 50 ms, respectively, to allow 10 Hz
  // operation (as suggested by table "Interleaved mode
  // limits (10 Hz operation)" in the datasheet).
  //s_vl6180X.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
  //s_vl6180X.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
  s_vl6180X.setScaling(3);
  

  s_vl6180X.setTimeout(500);

   // stop continuous mode if already active
  s_vl6180X.stopContinuous();
  s_vl6180X.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 254);
    s_vl6180X.writeReg(VL6180X::SYSRANGE__RANGE_IGNORE_VALID_HEIGHT, 150);
  s_vl6180X.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 500);
  // in case stopContinuous() triggered a single-shot
  // measurement, wait for it to complete
  delay(300);
  // start interleaved continuous mode with period of 100 ms
  //s_vl6180X.startInterleavedContinuous(100);
        xSemaphoreGive(xSemaphoreX);    


xTaskCreate(TaskVL6180X,"VL6180X",10000,NULL,0,NULL);
syslog_ng("VL6180X add Task");

#endif //c_VL6180X_us