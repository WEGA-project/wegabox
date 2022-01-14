#if c_EC == 1
// Измерение ЕС и NTC
void TaskEC(void *parameters)
{
  for (;;)
  {
    // if (xSemaphoreTake() == pdTRUE)
    // {
      //long s;
      unsigned long t_EC0;

      unsigned long An0 = 0;
      unsigned long Ap0 = 0;
      ECwork = true;

      adc1_config_width(ADC_WIDTH_BIT_12);
      adc1_config_channel_atten(EC_AnalogPort, ADC_ATTEN_DB_11);
      
      pinMode(EC_DigitalPort1, OUTPUT);
      pinMode(EC_DigitalPort2, OUTPUT);
      

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
      ECwork = false;
      float Mid_Ap0 = float(Ap0) / EC_MiddleCount;
      float Mid_An0 = float(An0) / EC_MiddleCount;

      if (Mid_Ap0 < 4095)
        Ap = Mid_Ap0;
      if (Mid_An0 > 0)
        An = Mid_An0;

        vTaskDelay(10000 / portTICK_PERIOD_MS);
    //   xSemaphoreGive(xSemaphore);
    // }

    
  }
}
#endif // c_EC