#if c_VL53L0X_us == 1
void TaskVL53L0X(void *parameters)
{

  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    delay(100);
    unsigned long VL53L0X_LastTime = millis() - VL53L0X_old;

    if (xSemaphoreX != NULL and VL53L0X_LastTime > VL53L0X_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        unsigned long VL53L0X_time = millis();
        syslog_ng("VL53L0X Start " + fFTS(VL53L0X_LastTime - VL53L0X_Repeat, 0) + "ms");

        Wire.begin(13, 14);
        

        // syslog_ng("VL53L0X Range:" + fFTS(s_VL53L0X.readRangeSingleMillimeters(), 0));

        // delay(100);
        long err = 0;
        float range = 0;
        float range0 = 0;
        unsigned cont = 0;
        unsigned long t = millis();

        // delay(500);
        // Dist = float(s_VL53L0X.readRangeContinuousMillimeters())/10;
        while (millis() - t < 15000)
        {
             cont++;
          //delay(10);
          if (!s_VL53L0X.timeoutOccurred())
            //range0 = s_VL53L0X.readRangeContinuousMillimeters();
            range0 = s_VL53L0X.readRangeSingleMillimeters();
            if (range0 and range0 != 65535 and range0 !=0 )
            {
             range = range + range0;
             //cont++;
            }
           else
           {
             err++;

           }

          //   // syslog_err("VL53L0X dist: restart...");
          //   // Wire.begin(13, 14);
          //   // s_VL53L0X.stopContinuous();
          //   // delay(100);
          //   // s_VL53L0X.setTimeout(500);
          //   // s_VL53L0X.init();
          //   // delay(100);
          //   // //s_VL53L0X.setMeasurementTimingBudget(200000);

          //   // // Start continuous back-to-back mode (take readings as
          //   // // fast as possible).  To use continuous timed mode
          //   // // instead, provide a desired inter-measurement period in
          //   // // ms (e.g. sensor.startContinuous(100)).
          //   // s_VL53L0X.startContinuous(10);

          // }
          //     VL53L0X_RangeRM.add(range0);
          //   else
          //     {err++;
          //     s_VL53L0X.init();
          //     s_VL53L0X.stopContinuous();
          //     delay(100);
          //     s_VL53L0X.startContinuous();
          //     }
        }
        //Dist = range / cont / 10;

         VL53L0X_RangeRM.add(range / (cont - err) /10);
         Dist = VL53L0X_RangeRM.getAverage(6);

        VL53L0X_time = millis() - VL53L0X_time;

        syslog_ng("VL53L0X dist: " + fFTS(Dist, 3));
        syslog_ng("VL53L0X Error/Count: " + String(err) + "/" + String(cont));
        // syslog_ng("VL53L0X Highest: " + fFTS(VL53L0X_RangeRM.getHighest(), 1));
        // syslog_ng("VL53L0X Lowest: " + fFTS(VL53L0X_RangeRM.getLowest(), 1));

        syslog_ng("VL53L0X " + fFTS(VL53L0X_time, 0) + "ms end.");

        Wire.begin(I2C_SDA, I2C_SCL, 100000L);
        VL53L0X_old = millis();
        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif // c_VL53L0X_us