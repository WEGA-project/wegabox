#if c_ADS1115 == 1  
  adc.reset();
  delay (100);
  adc.init();
  
      adc.setMeasureMode(ADS1115_CONTINUOUS); 
      adc.setCompareChannels(ADS1115_COMP_0_3);
      adc.setVoltageRange_mV(ADS1115_RANGE_4096);
      adc.setConvRate(ADS1115_860_SPS);
  delay (1000);

xTaskCreate(TaskADS1115,"ADS1115",10000,NULL,0,NULL);
  delay (1000);

#endif // c_ADS1115