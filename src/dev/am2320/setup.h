#if c_AM2320 == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE);

  if (! AM2320.begin() )
  {
    Serial.println("AM2320 Sensor not found");      
  }
xSemaphoreGive(xSemaphoreX);

xTaskCreate(TaskAM2320,"AM2320",10000,NULL,0,&appTasks[appTaskCount++]);
#endif // c_AM2320