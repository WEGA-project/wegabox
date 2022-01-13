#if c_PR == 1
pinMode(PR_AnalogPort, INPUT);
adc1_config_width(ADC_WIDTH_BIT_12);
adc1_config_channel_atten(PR_AnalogPort, ADC_ATTEN_DB_11);
xTaskCreate(TaskPR, "TaskPR", 10000, NULL, 0, &appTasks[appTaskCount++]);
#endif // c_PR