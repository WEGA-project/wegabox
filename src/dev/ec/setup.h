
#if c_EC == 1

        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(EC_AnalogPort, ADC_ATTEN_DB_11);
        delay (1000);
        xTaskCreate(TaskEC,"TaskEC",10000,NULL,0,&appTasks[appTaskCount++]);
        delay (1000);
#endif
