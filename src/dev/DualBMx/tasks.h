#if c_DualBMx == 1
void TaskDualBMx(void *parameters)
{
  for (;;)
  { //
    if (OtaStart == true)
      vTaskDelete(NULL);
    //syslog_ng("BMP280 loop");  
    vTaskDelay(1000);

    unsigned long DualBMx_LastTime = millis() - DualBMx_old;

    if (xSemaphoreX != NULL and DualBMx_LastTime > DualBMx_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long DualBMx_time = millis();
        syslog_ng("DualBMx Start " + fFTS(DualBMx_LastTime - DualBMx_Repeat, 0) + "ms");

        bmx280_1.measure();
        bmx280_2.measure();
        delay(100);
      float press_1, press_2;
      if (bmx280_1.hasValue() == true){
        press_1 = bmx280_1.getPressure64();
        if (bmx280_2.hasValue() == true){
          press_2 = bmx280_2.getPressure64();
          DstRM.add((press_1-press_2)*0.101972);          
        }
      }
        Dist=DstRM.getMedian();
        DualBMx_time = millis() - DualBMx_time;

        syslog_ng("bmx280_1 AirPress (Pa):" + fFTS(press_1, 3));   
        syslog_ng("bmx280_2 AirPress (Pa):" + fFTS(press_2, 3));  
        syslog_ng("DualBMx Dist (mm H2O):" + fFTS(Dist, 2));

        DualBMx_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}

#endif //c_DualBMx