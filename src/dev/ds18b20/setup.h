#if c_DS18B20 == 1
    sens18b20.begin();
    sens18b20.setResolution(12);
    xTaskCreate(TaskDS18B20,"TaskDS18B20",10000,NULL,0,NULL);
#endif // c_DS18B20