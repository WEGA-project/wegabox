#if c_NTC == 1


xTaskCreate(TaskNTC,"TaskNTC",5000,NULL,1,NULL);
syslog_ng("NTC add Task");
#endif
