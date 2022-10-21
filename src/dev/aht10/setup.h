#if c_AHT10 == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE);

  myAHT10.softReset();
  delay(50);
  myAHT10.begin();
  myAHT10.setNormalMode();
  //myAHT10.setCycleMode();
xSemaphoreGive(xSemaphoreX);


xTaskCreate(TaskAHT10,"TaskAHT10",5000,NULL,1,NULL);
syslog_ng("AHT10 add Task");
#endif // c_TaskAHT10
