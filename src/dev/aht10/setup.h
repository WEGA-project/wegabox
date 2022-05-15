#if c_AHT10 == 1
  myAHT10.softReset();
  delay(50);
  myAHT10.begin();
  myAHT10.setNormalMode();
  //myAHT10.setCycleMode();

delay (100);
xTaskCreate(TaskAHT10,"TaskAHT10",5000,NULL,1,NULL);
syslog_ng("AHT10 add Task");
#endif // c_TaskAHT10
