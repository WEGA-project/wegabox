// Измерение ЕС и NTC
void TaskEC(void *parameters)
{
  for (;;)
  {

    if (OtaStart == true)
    {
      vTaskDelete(NULL);
    }
    else
    {

#if c_EC == 1

      long s;
      unsigned long t_EC0;
if ( xSemaphoreTake( xI2CSemaphore, ( TickType_t ) 5 ) == pdTRUE )      {
      //pinMode(EC_AnalogPort, INPUT);
      unsigned long An0 = 0;
      unsigned long Ap0 = 0;

      pinMode(EC_DigitalPort1, OUTPUT);
      pinMode(EC_DigitalPort2, OUTPUT);
      ECwork = true;

      adc1_config_width(ADC_WIDTH_BIT_12);
      adc1_config_channel_atten(EC_AnalogPort, ADC_ATTEN_DB_11);

      t_EC0 = micros();
      digitalWrite(EC_DigitalPort1, LOW);
      digitalWrite(EC_DigitalPort2, LOW);

      // static portMUX_TYPE my_mutex = portMUX_INITIALIZER_UNLOCKED;
      rtc_wdt_protect_off();
      rtc_wdt_disable();
      disableCore0WDT();
      disableLoopWDT();
      // vPortCPUInitializeMutex(&my_mutex);
      long ect = millis();
      for (long i = 0; i < EC_MiddleCount and OtaStart != true; i++)
      {

        pinMode(EC_DigitalPort1, OUTPUT);
        pinMode(EC_DigitalPort2, OUTPUT);
        digitalWrite(EC_DigitalPort1, LOW);
        digitalWrite(EC_DigitalPort2, LOW);

        //portENTER_CRITICAL(&my_mutex);
        digitalWrite(EC_DigitalPort1, HIGH);
        Ap0 = adc1_get_raw(EC_AnalogPort) + Ap0;
        digitalWrite(EC_DigitalPort1, LOW);
          delayMicroseconds(1);
          
        digitalWrite(EC_DigitalPort2, HIGH);
          delayMicroseconds(1);
        digitalWrite(EC_DigitalPort2, LOW);
          //delayMicroseconds(1);

        digitalWrite(EC_DigitalPort1, HIGH);
          delayMicroseconds(1);
        digitalWrite(EC_DigitalPort1, LOW);
          //delayMicroseconds(1);

        digitalWrite(EC_DigitalPort2, HIGH);
        An0 = adc1_get_raw(EC_AnalogPort) + An0;
        digitalWrite(EC_DigitalPort2, LOW);
          //delayMicroseconds(1);

        //portEXIT_CRITICAL(&my_mutex);

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

      //esp_task_wdt_reset();

      float Mid_Ap0 = float(Ap0) / EC_MiddleCount;
      float Mid_An0 = float(An0) / EC_MiddleCount;


      ECwork = false;
      f_EC = EC_MiddleCount / (float(t_EC) / 1000000);

      if (Mid_Ap0 < 4095)
        Ap = Mid_Ap0;
      if (Mid_An0 > 0)
        An = Mid_An0;
  
  xSemaphoreGive(xI2CSemaphore);}
  vTaskDelay(1000 / portTICK_PERIOD_MS);    

#endif // c_EC

// Измерение термистора
#if c_NTC == 1
      adc1_config_channel_atten(NTC_port, ADC_ATTEN_DB_11);
      
      unsigned long NTC0 = 0;
      s = 0;
      
      while (s < NTC_MiddleCount and OtaStart != true)

      {
        
        float adc_ntc=adc1_get_raw(NTC_port);
        NTC0 = adc_ntc + NTC0;
        s++;
      }
      rtc_wdt_protect_on();
      rtc_wdt_enable();
      enableCore0WDT();
      enableLoopWDT();

          NTCRM.add(NTC0/s);
          NTC=NTCRM.getAverage();

      vTaskDelay(1000 / portTICK_PERIOD_MS);
#endif // c_NTC
    }
  }
}