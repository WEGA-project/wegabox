
#if c_EC == 1

if (adc1_get_raw(NTC_port) > 0)
{

   __wega_adcAttachPin(EC_AnalogPort);
   //adc_power_acquire();
  // long EC_TaskErr = xTaskCreate(TaskEC, "TaskEC", 5000, NULL, 2, NULL);
   long EC_TaskErr = xTaskCreatePinnedToCore (TaskEC, "TaskEC", 5000, NULL, 2, NULL, 1);

   //}

   syslog_ng("EC: add Task:" + fFTS(EC_TaskErr, 0));
}

else
   syslog_err("EC: The electrode is not detected");
#endif
