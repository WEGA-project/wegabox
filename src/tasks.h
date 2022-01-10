// В этом файле расположены функции выполняющиеся параллельно freeRTOC

void TaskOTA(void * parameters){
  for(;;){
    server.handleClient();
    ArduinoOTA.handle();
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// Измерение фоторезистора
#if c_PR == 1
void TaskPR(void *parameters)
{
  pinMode(PR_AnalogPort, INPUT);
  
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(PR_AnalogPort, ADC_ATTEN_DB_11);

  for (;;)
  {
    if (OtaStart == true)
    {
      vTaskDelete(NULL);
    }
    else
    {
          PR = PRMediana.filtered ( adc1_get_raw(PR_AnalogPort));
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}
#endif //  c_PR

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

      vTaskDelay(2000 / portTICK_PERIOD_MS);
#endif // c_NTC
    }
  }
}

#if c_US025 == 1
void TaskUS(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);

    else
    {
        float us = distanceSensor.measureDistanceCm(25);
        
        if (us > 1) {
        Dist= DstMediana.filtered (us);
        Serial.println(us);
        }
        else  {
          Serial.print("US-025 Error us=");
          Serial.println(us);
          }
              
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}
#endif // c_US025

#if c_hall == 1
void TaskHall(void * parameters) {
  for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{

    hall=HallGAB.filtered( hallRead() );
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}
}
#endif //c_hall

#if c_CPUTEMP == 1
void TaskCPUtemp(void * parameters) {
  for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
    
     int CPUTemp0 = temprature_sens_read();
     
     CPUTemp=CpuTempKalman.filtered( ( CPUTemp0 - 32 )/1.8 );
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
  }
}
}
#endif //c_CPUTEMP

#if c_AHT10 == 1
void TaskAHT10(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    else
    {
      if (xSemaphoreTake(xI2CSemaphore, (TickType_t)5) == pdTRUE)
      {
        readStatus = myAHT10.readRawData();
        if (readStatus != AHT10_ERROR)
        {
          float AirTemp0 = myAHT10.readTemperature();
          float AirHum0 = myAHT10.readHumidity();
          if (AirTemp0 != 255 and AirHum0 != 255 and AirTemp0 != -50)
          {
            AirTemp = AirTempMediana.filtered(AirTemp0);
            AirHum = AirHumMediana.filtered(AirHum0);
          }
        }
        else
        {
          myAHT10.softReset();
          vTaskDelay(200 / portTICK_PERIOD_MS);
          myAHT10.begin();
          myAHT10.setNormalMode();
          //myAHT10.setCycleMode();
        }
        xSemaphoreGive(xI2CSemaphore);
      }
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }
}
#endif // c_AHT10

#if c_CCS811 == 1
void TaskCCS811(void *parameters)
{
  for (;;)
  {
    if (xSemaphoreTake(xI2CSemaphore, (TickType_t)5) == pdTRUE)
    {
      // Read
      ccs811.set_envdata_Celsius_percRH(AirTemp, AirHum);
      uint16_t eco2, etvoc, errstat, raw;
      ccs811.read(&eco2, &etvoc, &errstat, &raw);

      // Print measurement results based on status
      if (errstat == CCS811_ERRSTAT_OK)
      {
        CO2 = CO2Mediana.filtered(eco2);
        tVOC = etvoc;
        eRAW = raw;
      }
      
      xSemaphoreGive(xI2CSemaphore);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
#endif

#if c_MCP3421 == 1
void TaskMCP3421(void *parameters)
{
  long adcvalue;
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    else
    {
      if (xSemaphoreTake(xI2CSemaphore, (TickType_t)5) == pdTRUE)
      {
        MCP342x::Config status;
        long phvalue;
        uint8_t err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot, MCP342x::resolution18, MCP342x::gain4, 100000, phvalue, status);
        if (!err)
        {

          //pHraw = phvalue;
          //pHmV = PhMediana.filtered(4096 / pow(2, 18) * pHraw / 4);
          pHraw = phvalue;
          PhRM.add(phvalue);
          pHmV=(4096 / pow(2, 18) *PhRM.getMedian()/ 4);
        }
        xSemaphoreGive(xI2CSemaphore);
      }
    vTaskDelay(2000 / portTICK_PERIOD_MS);  
    }
  }
}
#endif // c_MCP3421

#if c_DS18B20 == 1
void TaskDS18B20(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
    {
      vTaskDelete(NULL);
    }
    else
    {
      //vTaskDelay(2000 / portTICK_PERIOD_MS);
      //sens18b20.begin();
      
      sens18b20.requestTemperatures();
      float ds0 = sens18b20.getTempCByIndex(0);
      if (ds0 != -127 and ds0 != 85)        {
        RootTemp = RootTempMediana.filtered(ds0);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      } else sens18b20.begin();
    vTaskDelay(20 / portTICK_PERIOD_MS);
    }
  }
}
#endif //c_DS18B20

#if c_ADS1115 == 1
  void TaskADS1115(void * parameters) {
    for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
      
            if ( xSemaphoreTake( xI2CSemaphore, ( TickType_t ) 5 ) == pdTRUE )      {
      //adc.reset();   
      //delay(100);
        adc.init();
        delay(500);
      
      //adc.setConvRate(ADS1115_16_SPS);
      //adc.setVoltageRange_mV(ADS1115_RANGE_4096);
      adc.setMeasureMode(ADS1115_CONTINUOUS); 
      //    adc.setMeasureMode(ADS1115_SINGLE);
      adc.setCompareChannels(ADS1115_COMP_0_3);
      adc.setVoltageRange_mV(ADS1115_RANGE_4096);
      adc.setConvRate(ADS1115_860_SPS);
      long cont=0;
      double sensorValue=0;
      while ( cont < ADS1115_MiddleCount){
       cont++;
        sensorValue =  adc.getResult_mV()+sensorValue;
      }
      //pHmV=PhMediana.filtered(sensorValue/cont);
      //pHmV=PhMediana.filtered(adc.getResult_mV());
      
      PhRM.add(sensorValue/cont);
      pHmV=PhRM.getAverage();
      pHraw=adc.getRawResult();
      xSemaphoreGive( xI2CSemaphore );      }
    }
  vTaskDelay(5000 / portTICK_PERIOD_MS);  
  }
}
#endif // c_ADS1115

#if c_AM2320 == 1
void TaskAM2320(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);

    else
    {
      vTaskDelay(2000 / portTICK_PERIOD_MS);

      if (AM2320.begin())
      {

        int status = AM2320.read();
        switch (status)
        {
        case AM232X_OK:
          AirHum = AirHumMediana.filtered(AM2320.getHumidity());
          AirTemp = AirTempMediana.filtered(AM2320.getTemperature());
          break;
        default:
          Serial.println(status);
          break;
        }
      }
    }
  }
}
#endif //c_AM2320

#if c_BME280 == 1
  void TaskBME280(void * parameters) {
  for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{  
    vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
    AirTemp=bme.readTemperature();
    AirHum=bme.readHumidity();
    AirPress=bme.readPressure()*0.00750062;
    }
  }
  }
#endif //c_BME280

#if c_BMP280 == 1
  void TaskBMP280(void *parameters)
  {
    for (;;)
    {
      if (OtaStart == true)
      {
        vTaskDelete(NULL);
      }
      else
      {

        if (xSemaphoreTake(xI2CSemaphore, (TickType_t)5) == pdTRUE)
        {

          if (!bmx280.measure())
          {
            Serial.println("could not start measurement, is a measurement already running?");
            return;
          }

          //wait for the measurement to finish
          do
          {
            delay(100);
          } while (!bmx280.hasValue());

          AirTemp = AirTempMediana.filtered(bmx280.getTemperature());
          AirPress = AirPressMediana.filtered(bmx280.getPressure64() * 0.00750063755419211);

          if (bmx280.isBME280()) // Если датчик BME280 еще и влажность определить
          {
            AirHum = AirHumMediana.filtered(bmx280.getHumidity());
          }

          xSemaphoreGive(xI2CSemaphore);
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      }
    }
  }
#endif //c_BMP280

#if c_HX710B == 1
  void TaskHX710B(void *parameters)
  {
    for (;;)
    {
      if (OtaStart == true)
      {
        vTaskDelete(NULL);
      }
      else
      {
            uint32_t data_raw = 0;
            if ( air_press.read(&data_raw, 1000UL) == HX710B_OK )
            {
              Serial.print(F("Data raw of ADC is : "));
              Serial.println((unsigned long) data_raw);
              //Dist=DstMediana.filtered(data_raw);
              DstRM.add(data_raw);
              Dist=DstRM.getMedian();
              Serial.println(Dist);
            }
      
       
      //  unsigned long cont=0;
      //  uint32_t data_raw = 0;
      //  long Dist0=0;
      //  while ( cont < 100){
      //    if ( air_press.read(&data_raw, 1000UL) == HX710B_OK ){
      //  cont++;
      //  Dist0=data_raw+Dist0;
      //  }
      // }
      // Dist=float(Dist0/cont);
      // Serial.print(F("Data raw of ADC is : "));
      // Serial.println(Dist);
       }
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }

#endif //c_HX710B

#if c_MCP23017 == 1

  void TaskMCP23017(void *parameters)
  {
    for (;;)
    {
      if (OtaStart == true)
      {
        vTaskDelete(NULL);
      }
      else
      {


      //pinMode(PWD1, OUTPUT);


      digitalWrite(PWD1, HIGH);

      //pinMode(PWD2, OUTPUT);
      //digitalWrite(PWD2, HIGH);

      mcp.pinMode(DRV1_A, OUTPUT);
      //mcp.pinMode(DRV1_B, OUTPUT);
      //mcp.pinMode(DRV1_C, OUTPUT);
      //mcp.pinMode(DRV1_D, OUTPUT);

      mcp.digitalWrite(DRV1_A, HIGH);
      //Serial.println("DRV1_A START");
      long pw=255;
      Serial.println(pw);
      ledcWrite(0, pw);
        delay (500);

      // pw=2;
      // Serial.println(pw);
      // ledcWrite(0, pw);
      //   delay (10000);
     for (int pw=3;pw<200;pw++){
         Serial.println(pw);
       ledcWrite(0, pw);
       delay (1000);
      }

      // pw=20;
      // Serial.println(pw);
      // ledcWrite(0, pw);
      //   delay (5000);

      // pw=50;
      // Serial.println(pw);
      // ledcWrite(0, pw);
      //   delay (5000);

      // pw=100;
      // Serial.println(pw);
      // ledcWrite(0, pw);
      //   delay (5000);

      // pw=200;
      // Serial.println(pw);
      // ledcWrite(0, pw);
      //   delay (5000);

      pw=254;
      Serial.println(pw);
      ledcWrite(0, pw);
        delay (5000);
        
      // for (int pw=10;pw<254;pw++){
      //   Serial.println(pw);
      // ledcWrite(0, pw);
      // delay (50);
      //  }
      // Serial.println("DRV1_A STOP"); 
      // for (int pw=254;pw>10;pw--){
      //   Serial.println(pw);
      // ledcWrite(0, pw);
      // delay (50);
      
      }


      vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
  }

#endif // c_MCP23017