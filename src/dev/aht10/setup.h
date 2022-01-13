#if c_AHT10 == 1
  myAHT10.softReset();
  delay(50);
  myAHT10.begin();
  myAHT10.setNormalMode();
  //myAHT10.setCycleMode();


xTaskCreate(TaskAHT10,"TaskAHT10",10000,NULL,0,NULL);
#endif // c_TaskAHT10
