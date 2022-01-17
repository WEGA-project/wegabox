#if c_EC == 1
// Измерение ЕС 
void TaskEC(void *parameters)
{
  for (;;)
  {
    

    if (xSemaphoreX != NULL and millis()-EC_old > EC_Repeat)
    { 
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)10) == pdTRUE)
      { syslog.log(LOG_INFO, fFTS ( (millis()-EC_old), 0) + " EC REPEAT" );
        syslog.log(LOG_INFO, fFTS(millis() / 1000, 3) + " sec Start EC");

        unsigned long t_EC0;

        unsigned long An0 = 0;
        unsigned long Ap0 = 0;
        ECwork = true;


        pinMode(EC_DigitalPort1, OUTPUT);
        pinMode(EC_DigitalPort2, OUTPUT);

        t_EC0 = micros();
        digitalWrite(EC_DigitalPort1, LOW);
        digitalWrite(EC_DigitalPort2, LOW);

        long ect = millis();
        disableCore0WDT();
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
        
        enableCore0WDT();
        t_EC = micros() - t_EC0;
        pinMode(EC_DigitalPort1, INPUT);
        pinMode(EC_DigitalPort2, INPUT);
        ECwork = false;
        float Mid_Ap0 = float(Ap0) / EC_MiddleCount;
        float Mid_An0 = float(An0) / EC_MiddleCount;

        if (Mid_Ap0 < 4095)
          Ap = Mid_Ap0;
        if (Mid_An0 > 0)
          An = Mid_An0;
        
        xSemaphoreGive(xSemaphoreX);
        EC_old=millis();
        syslog.log(LOG_INFO, fFTS(millis() / 1000, 3) + " sec end EC");

      }
    }
      
      vTaskDelay(10000 / portTICK_PERIOD_MS);

  }
}
#endif // c_EC