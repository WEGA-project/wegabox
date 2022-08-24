#if c_VL53L0X_us == 1


  Wire.begin(13,14);

  s_VL53L0X.init();
  //s_VL53L0X.stopContinuous();
  s_VL53L0X.setTimeout(500);
  vTaskDelay(100);
  //s_VL53L0X.startContinuous();

// #if defined LONG_RANGE
  //lower the return signal rate limit (default is 0.25 MCPS)
   s_VL53L0X.setSignalRateLimit(0.01);
  // // increase laser pulse periods (defaults are 14 and 10 PCLKs)
 s_VL53L0X.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
 s_VL53L0X.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
// #endif
  
// #if defined HIGH_SPEED
//   // reduce timing budget to 20 ms (default is about 33 ms)
//   s_VL53L0X.setMeasurementTimingBudget(20000);
// #elif defined HIGH_ACCURACY
//   // increase timing budget to 200 ms
   s_VL53L0X.setMeasurementTimingBudget(100);
// #endif


xTaskCreate(TaskVL53L0X,"VL53L0X",10000,NULL,0,NULL);
syslog_ng("VL53L0X add Task");

#endif //c_VL53L0X_us