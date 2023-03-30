// Измерение термистора
#if c_EC == 1
void TaskEC(void *parameters)
{
        syslog_ng("EC Task Start");
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(EC_AnalogPort, ADC_ATTEN_DB_11); 
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(100);
    //syslog_ng("EC loop");

    unsigned long EC_LastTime = millis() - EC_old;

    if (xSemaphoreX != NULL and EC_LastTime > EC_Repeat)
    { //syslog_ng("EC Semaphore");
      vTaskDelay(1);

      if (xSemaphoreTake(xSemaphoreX, (TickType_t)2) == pdTRUE)
      {
        unsigned long EC_time = millis();
        syslog_ng("EC Start " + fFTS(EC_LastTime - EC_Repeat, 0) + "ms");

        unsigned long An0 = 0;
        unsigned long Ap0 = 0;




        __wega_adcAttachPin(33);
        adc_power_acquire();
        SAR_ADC1_LOCK_ACQUIRE();

        pinMode(EC_DigitalPort1, OUTPUT);
        pinMode(EC_DigitalPort2, OUTPUT);

        digitalWrite(EC_DigitalPort1, LOW);
        digitalWrite(EC_DigitalPort2, LOW);
        long ect = millis();
        for (long i = 0; i < EC_MiddleCount and OtaStart != true; i++)
        {

          digitalWrite(EC_DigitalPort1, HIGH);
          __wega_adcStart(33);
          Ap0 = __wega_adcEnd(33) + Ap0; 
          digitalWrite(EC_DigitalPort1, LOW);

          digitalWrite(EC_DigitalPort2, HIGH);
          __wega_adcStart(33);
          An0 = __wega_adcEnd(33) + An0; 
          digitalWrite(EC_DigitalPort2, LOW);
        }
        pinMode(EC_DigitalPort1, INPUT);
        pinMode(EC_DigitalPort2, INPUT);

        SAR_ADC1_LOCK_RELEASE();
        adc_power_release();

        float Mid_Ap0 = float(Ap0) / EC_MiddleCount;
        float Mid_An0 = float(An0) / EC_MiddleCount;
        ApRM.add(Mid_Ap0);
        AnRM.add(Mid_An0);
        if (Mid_Ap0 < 4095)
          //Ap = Mid_Ap0;
          Ap = ApRM.getAverage();
        if (Mid_An0 > 0)
          //An = Mid_An0;
          An = AnRM.getAverage();

        EC_time = millis() - EC_time;
        syslog_ng("EC Ap:" + fFTS(Ap, 3));
        syslog_ng("EC An:" + fFTS(An, 3));
        syslog_ng("EC " + fFTS(EC_time, 0) + "ms end.");
        EC_old = millis();
        
        xSemaphoreGive(xSemaphoreX);
        //vTaskDelay(1000 / portTICK_PERIOD_MS);
      }
    }
  }
}
#endif // c_EC