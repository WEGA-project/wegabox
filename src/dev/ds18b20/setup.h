#if c_DS18B20 == 1

sens18b20.begin();
sens18b20.begin();

if (sens18b20.getDeviceCount() > 0)
{
    sens18b20.setResolution(12);
    syslog_ng("DS18B20 Found 1-W devices:" + fFTS(sens18b20.getDeviceCount(), 0));
}
else
{
    syslog_ng("DS18B20 not found!");
}

long DS18B20_TaskErr = xTaskCreate(TaskDS18B20, "TaskDS18B20", 10000, NULL, 1, NULL);
syslog_ng("DS18B20 add Task:" + fFTS(DS18B20_TaskErr, 0));

#endif // c_DS18B20