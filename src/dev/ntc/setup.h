#if c_NTC == 1
adc1_config_width(ADC_WIDTH_BIT_12);
adc1_config_channel_atten(NTC_port, ADC_ATTEN_DB_11);

xTaskCreate(TaskNTC,"TaskNTC",100000,NULL,1,&appTasks[appTaskCount++]);
#endif
