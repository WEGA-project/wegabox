
#if c_EC == 1
//long EC_TaskErr = -1;

// while (EC_TaskErr == -1)
// {
   long  EC_TaskErr =  xTaskCreate(TaskEC, "TaskEC", 5000, NULL, 2,NULL);
//}

syslog_ng("EC add Task:" + fFTS(EC_TaskErr, 0));
#endif
