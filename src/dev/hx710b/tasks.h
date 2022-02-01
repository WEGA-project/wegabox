#if c_HX710B == 1
void TaskHX710B(void *parameters)
{

  for (;;)
  { //
    if (OtaStart == true)
      vTaskDelete(NULL);
    //syslog_ng("HX710B loop");
    vTaskDelay(1000);

    unsigned long HX710B_LastTime = millis() - HX710B_old;

    if (xSemaphoreX != NULL and HX710B_LastTime > HX710B_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        unsigned long HX710B_time = millis();
        syslog_ng("HX710B Start " + fFTS(HX710B_LastTime - HX710B_Repeat, 0) + "ms");

        uint32_t data_raw = 0;
        long cont = 0;
        long HX710B_err = HX710B_BUSY;
        while (HX710B_err != HX710B_OK and cont < 30)
        {
          HX710B_err = HX710B_Press.read(&data_raw, 1000UL);
          cont++;

          delay(100);
        }
        if (data_raw != 0)
        {
          HX710B_DistRM.add(HX710B_a + float(data_raw) * HX710B_b);
          Dist = HX710B_DistRM.getAverage(10);
          DstRAW = data_raw;
        
          syslog_ng("HX710B Dist:" + fFTS(Dist, 4));
          syslog_ng("HX710B Pressure RAW:" + fFTS(DstRAW, 0) + " cont:" + fFTS(cont, 0));

        }
        else
          syslog_ng("HX710B Error. Please check sensor");

        HX710B_time = millis() - HX710B_time;
        syslog_ng("HX710B " + fFTS(HX710B_time, 0) + "ms end.");
        HX710B_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif //c_HX710B