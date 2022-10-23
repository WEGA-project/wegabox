
#if c_EC == 1

if (adc1_get_raw(NTC_port) > 0)
{

   long EC_TaskErr = xTaskCreate(TaskEC, "TaskEC", 5000, NULL, 2, NULL);
   //}

   syslog_ng("EC: add Task:" + fFTS(EC_TaskErr, 0));
}

else
   syslog_err("EC: The electrode is not detected");
#endif
