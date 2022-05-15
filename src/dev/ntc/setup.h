#if c_NTC == 1
adc1_config_width(ADC_WIDTH_BIT_12);
adc1_config_channel_atten(NTC_port, ADC_ATTEN_DB_11);

xTaskCreate(TaskNTC,"TaskNTC",5000,NULL,0,NULL);
syslog_ng("NTC add Task");
#endif
