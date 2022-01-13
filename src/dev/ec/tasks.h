#if c_EC == 1
// Измерение ЕС и NTC
void TaskEC(void *parameters)
{
  for (;;)
  {
    if (xSemaphore != NULL)
    {
      if (xSemaphoreTake(xSemaphore, (TickType_t)30) == pdTRUE)
      {
        //long s;
        unsigned long t_EC0;

        unsigned long An0 = 0;
        unsigned long Ap0 = 0;

        pinMode(EC_DigitalPort1, OUTPUT);
        pinMode(EC_DigitalPort2, OUTPUT);
        ECwork = true;

        t_EC0 = micros();
        digitalWrite(EC_DigitalPort1, LOW);
        digitalWrite(EC_DigitalPort2, LOW);

        long ect = millis();
        for (long i = 0; i < EC_MiddleCount and OtaStart != true; i++)
        {

          pinMode(EC_DigitalPort1, OUTPUT);
          pinMode(EC_DigitalPort2, OUTPUT);
          digitalWrite(EC_DigitalPort1, LOW);
          digitalWrite(EC_DigitalPort2, LOW);

          digitalWrite(EC_DigitalPort1, HIGH);
          Ap0 = adc1_get_raw(EC_AnalogPort) + Ap0;
          digitalWrite(EC_DigitalPort1, LOW);
          delayMicroseconds(1);

          digitalWrite(EC_DigitalPort2, HIGH);
          delayMicroseconds(1);
          digitalWrite(EC_DigitalPort2, LOW);

          digitalWrite(EC_DigitalPort1, HIGH);
          delayMicroseconds(1);
          digitalWrite(EC_DigitalPort1, LOW);

          digitalWrite(EC_DigitalPort2, HIGH);
          An0 = adc1_get_raw(EC_AnalogPort) + An0;
          digitalWrite(EC_DigitalPort2, LOW);

          if (millis() - ect > 1000)
          {
            pinMode(EC_DigitalPort1, INPUT);
            pinMode(EC_DigitalPort2, INPUT);
            vTaskDelay(300 / portTICK_PERIOD_MS);
            ect = millis();
          }
        }

        t_EC = micros() - t_EC0;
        pinMode(EC_DigitalPort1, INPUT);
        pinMode(EC_DigitalPort2, INPUT);

        float Mid_Ap0 = float(Ap0) / EC_MiddleCount;
        float Mid_An0 = float(An0) / EC_MiddleCount;

        // ECwork = false;
        // f_EC = EC_MiddleCount / (float(t_EC) / 1000000);

        if (Mid_Ap0 < 4095)
          Ap = Mid_Ap0;
        if (Mid_An0 > 0)
          An = Mid_An0;
        xSemaphoreGive(xSemaphore);
      }
    }
    // rtc_wdt_protect_on();
    // rtc_wdt_enable();
    // enableCore0WDT();
    // enableLoopWDT();

    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}
#endif // c_EC