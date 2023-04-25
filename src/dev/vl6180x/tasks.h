#if c_VL6180X == 1
void TaskVL6180X(void *parameters)
{

  if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
  {
    s_vl6180X.init();
    s_vl6180X.configureDefault();
    s_vl6180X.setScaling(1);
    s_vl6180X.setTimeout(500);
    s_vl6180X.stopContinuous();


#define SYSTEM__FRESH_OUT_OF_RESET 0x46 // адрес регистра
#define FRESH_OUT_OF_RESET_VALUE 0x01 // значение для сброса
// Отправляем сообщение в регистр SYSTEM__FRESH_OUT_OF_RESET, чтобы выполнить сброс датчика
s_vl6180X.writeReg(SYSTEM__FRESH_OUT_OF_RESET, FRESH_OUT_OF_RESET_VALUE);

  //   s_vl6180X.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 254);

  //   s_vl6180X.writeReg(VL6180X::SYSRANGE__RANGE_IGNORE_VALID_HEIGHT, 150);
  //   s_vl6180X.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 1500);

  // s_vl6180X.writeReg(VL6180X::SYSRANGE__THRESH_HIGH, 11);
  // s_vl6180X.writeReg(VL6180X::SYSRANGE__THRESH_LOW, 8);
  // s_vl6180X.writeReg(VL6180X::SYSRANGE__INTERMEASUREMENT_PERIOD, 9);
  // s_vl6180X.writeReg(VL6180X::SYSRANGE__RANGE_IGNORE_VALID_HEIGHT, 0);
  // s_vl6180X.writeReg(VL6180X::SYSRANGE__RANGE_IGNORE_THRESHOLD, 0);
  // s_vl6180X.writeReg(VL6180X::SYSRANGE__MAX_AMBIENT_LEVEL_MULT, 3);
  // s_vl6180X.writeReg(VL6180X::SYSRANGE__VHV_REPEAT_RATE, 255);
  s_vl6180X.writeReg(VL6180X::SYSRANGE__VHV_REPEAT_RATE, 10);
   s_vl6180X.writeReg16Bit(VL6180X::SYSRANGE__VHV_RECALIBRATE, 100);

       s_vl6180X.writeReg16Bit(VL6180X::SYSRANGE__CROSSTALK_COMPENSATION_RATE, 65535);
       s_vl6180X.writeReg16Bit(VL6180X::SYSRANGE__EARLY_CONVERGENCE_ESTIMATE, 65535);
   s_vl6180X.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 50);
   s_vl6180X.writeReg16Bit(VL6180X::SYSRANGE__INTERMEASUREMENT_PERIOD, 10000);

   s_vl6180X.writeReg16Bit(VL6180X::SYSTEM__GROUPED_PARAMETER_HOLD, 0);

// #include "init.h"


    xSemaphoreGive(xSemaphoreX);
  }

  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    delay(100);
    unsigned long VL6180X_LastTime = millis() - VL6180X_old;

    if (xSemaphoreX != NULL and VL6180X_LastTime > VL6180X_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)5) == pdTRUE)
      {
        unsigned long VL6180X_time = millis();
        syslog_ng("VL6180X Start " + fFTS(VL6180X_LastTime - VL6180X_Repeat, 0) + "ms");

        syslog_ng("VL6180X Range:" + fFTS(s_vl6180X.readRangeSingleMillimeters(), 0));

        s_vl6180X.setScaling(vl6180XScalling);
        // int32_t rangemm = s_vl6180X.readRangeSingleMillimeters();
        int32_t rangemm = s_vl6180X.readRangeSingleMillimeters();

        vl6180XScalling = 1;

        s_vl6180X.setScaling(vl6180XScalling);

        delay(100);
        long err = 0;
        float range0 = 0;
        unsigned cont = 0;
        unsigned long t = millis();

        range0 = s_vl6180X.readRangeSingleMillimeters();
        if (range0 != 255 and range0 != 0)
          VL6180X_RangeRM.add(range0);
        else
        {
          syslog_err("VL6180X: Error range");
          err++;
          Wire.endTransmission(1);
          Wire.end();
          delay(100);
          Wire.begin();
        }


        if (VL6180X_RangeRM.getAverage(25) != 0)
          Dist = VL6180X_RangeRM.getAverage(25) / 10;

        if (abs(Dist - range0/10) > 4)
        {
          VL6180X_RangeRM.clear();
          syslog_ng("VL6180X: Reset average filter Dist=" + String(Dist)+ " range="+String(range0));
          VL6180X_RangeRM.add(range0);
        }


        VL6180X_time = millis() - VL6180X_time;
        syslog_ng("VL6180X: dist=" + fFTS(Dist, 3));
        syslog_ng("VL6180X: range=" + String(rangemm) + " mm");
        syslog_ng("VL6180X: Scalling: " + String(vl6180XScalling));
        // syslog_ng("VL6180X: Error/Count " + String(err) + "/" + String(cont));
        syslog_ng("VL6180X: Highest= " + fFTS(VL6180X_RangeRM.getHighest(), 1));
        syslog_ng("VL6180X: Lowest= " + fFTS(VL6180X_RangeRM.getLowest(), 1));

        syslog_ng("VL6180X " + fFTS(VL6180X_time, 0) + "ms end.");
        VL6180X_old = millis();

        //#include "init.h"

        xSemaphoreGive(xSemaphoreX);
      }
    }
  }
}
#endif // c_VL6180X