 #if c_SDC30 == 1

  if (airSensor.begin() == false) //Pass the Wire port to the .begin() function
  {
    syslog_ng("SDC30: Air sensor not detected.");
  }
long SDC30_TaskErr = xTaskCreate(TaskSDC30,"TaskSDC30",8000,NULL,0,NULL);
syslog_ng("SDC30 add Task:" + fFTS(SDC30_TaskErr,0) );
#endif // c_SDC30  