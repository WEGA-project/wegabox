#if c_PR == 1
//pinMode(PR_AnalogPort, INPUT);
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(PR_AnalogPort, ADC_ATTEN_DB_11);


adc1_config_channel_atten(PR_AnalogPort, ADC_ATTEN_DB_0);
int PR_probe= adc1_get_raw(PR_AnalogPort);
adc1_config_channel_atten(PR_AnalogPort, ADC_ATTEN_DB_11);

syslog_ng("PR: Start probe = " + String(PR_probe) );

if (adc1_get_raw(PR_AnalogPort) > 0){

xTaskCreate(TaskPR, "TaskPR", 5000, NULL, 0,NULL);
syslog_ng("PR: add Task");
}
else
  syslog_err("PR: not detected");


#endif // c_PR