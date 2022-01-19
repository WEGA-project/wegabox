#if c_PR == 1
//pinMode(PR_AnalogPort, INPUT);

xTaskCreate(TaskPR, "TaskPR", 5000, NULL, 0,NULL);
syslog_ng("PR add Task");
#endif // c_PR