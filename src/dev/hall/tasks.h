#if c_hall == 1
void TaskHall(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(1000);

    unsigned long hall_LastTime = millis() - hall_old;

    if (xSemaphoreX != NULL and hall_LastTime > hall_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {
        vTaskDelay(2000);
        unsigned long hall_time = millis();
        syslog_ng("hall Start " + fFTS(hall_LastTime - hall_Repeat, 0) + "ms");

        float s_hall=0;
        for(long i=0;i<hall_MiddleCount;i++){

            vTaskDelay(1);
            s_hall=hallRead() +s_hall;

        }
            HallRM.add(s_hall/hall_MiddleCount);
            hall = HallRM.getAverage();
            //hall=s_hall/hall_MiddleCount;
        //hall = hallRead();


        hall_time = millis() - hall_time;
        syslog_ng("hall:" + fFTS(hall, 3));
        syslog_ng("hall: raw=" + fFTS(hallRead(), 3));
        syslog_ng("hall " + fFTS(hall_time, 0) + "ms end.");
        hall_old = millis();
        xSemaphoreGive(xSemaphoreX);
        syslog_ng("hall Semaphore Give");
      }
    }
  }
}

#endif //c_hall