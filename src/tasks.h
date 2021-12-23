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
      //unsigned long PR0=0;
      //unsigned long prt=millis();

      // for (long i = 0; i < PR_MiddleCount and OtaStart != true; i++)
      // {
        //PR0 = adc1_get_raw(PR_AnalogPort) + PR0;
          PR = PRMediana.filtered ( adc1_get_raw(PR_AnalogPort));

        // if (millis() - prt > 1000)
        // {
        //   vTaskDelay(100 / portTICK_PERIOD_MS);
        //   prt = millis();
        // }

      //}
      //PR = PRMediana.filtered (PR0 / PR_MiddleCount);
      //PR = float(PR0) / PR_MiddleCount;
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
        s++;

        NTC0 = NTCMediana.filtered(adc1_get_raw(NTC_port)) + NTC0;
        //esp_task_wdt_reset();
      }
      rtc_wdt_protect_on();
      rtc_wdt_enable();
      enableCore0WDT();
      enableLoopWDT();

      NTC =  float(NTC0) / s;
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
      // unsigned long  t_Dst0 = millis();
      // unsigned long UScnt = 0;
      // float dst0 = 0;

      // for (long i = 0; millis() - t_Dst0 < 180000; i++)
      // {
        // float us=-1;
        // while ( us == -1 ){
        // float us = distanceSensor.measureDistanceCm(25);
        // vTaskDelay(40 / portTICK_PERIOD_MS); 
        // }

        //Dist=DstMediana.filtered(us);
        float us = distanceSensor.measureDistanceCm(25);
        if (us > 1) Dist= DstMediana.filtered (us);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // if (us != -1)
        // {
        //   UScnt++;
        //   dst0 = dst0 + us;
        // }
        
       // vTaskDelay(4 / portTICK_PERIOD_MS);
      //}
     // Dist = dst0 / UScnt;
      //vTaskDelay(5000 / portTICK_PERIOD_MS);
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
      readStatus = myAHT10.readRawData();
      if (readStatus != AHT10_ERROR)
      {
        float AirTemp0 = myAHT10.readTemperature();
        float AirHum0 = myAHT10.readHumidity();
        if (AirTemp0 != 255 and AirHum0 != 255 and AirTemp0 != -50)
        {
          AirTemp = AirTempMediana.filtered(AirTemp0);
          AirHum = AirHumMediana.filtered(AirHum0);
          vTaskDelay(2000 / portTICK_PERIOD_MS);
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
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
  }
}
#endif // c_AHT10

#if c_CCS811 == 1
void TaskCCS811(void *parameters)
{
  for(;;){
    vTaskDelay(10000 / portTICK_PERIOD_MS);
      // Read
    ccs811.set_envdata_Celsius_percRH(AirTemp,AirHum);
    uint16_t eco2, etvoc, errstat, raw;
    ccs811.read(&eco2,&etvoc,&errstat,&raw); 
    
    // Print measurement results based on status
    if( errstat==CCS811_ERRSTAT_OK ) { 
      CO2=eco2;
      tVOC=etvoc;
      eRAW=raw;
    }
  }
} 
  #endif

#if c_MCP3421 == 1
void TaskMCP3421(void *parameters)
{

    //uint8_t err;
    
    long adcvalue;

  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    else
    {
      //
      //long pht = millis();

      
MCP342x::Config status;
      
      //long pHraw0=0;
      // long i=0;
      // long pH_MiddleCount=50;
     // while (i < pH_MiddleCount and OtaStart != true){
         
        //uint8_t err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot, MCP342x::resolution18, MCP342x::gain4, 100000, adcvalue, status);  
         //uint8_t err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot, MCP342x::resolution18, MCP342x::gain4, 100000, value, status);
          // while (adc.convertAndRead(MCP342x::channel1, MCP342x::continous, MCP342x::resolution18, MCP342x::gain4, 500000, adcvalue, status))
          //  vTaskDelay(5 / portTICK_PERIOD_MS);
          // pHraw0 = adcvalue;
        //  if (!err) { pHraw0 = adcvalue + pHraw0;
        //  i++;}
         
         //vTaskDelay(1 / portTICK_PERIOD_MS);
    //  }
   //   pHraw=pHraw0/pH_MiddleCount;
        long phvalue;
        uint8_t err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot, MCP342x::resolution18, MCP342x::gain4, 100000, phvalue, status);
      if (!err){
        pHraw= phvalue ;
      pHmV =  PhMediana.filtered(4096 / pow(2, 18) * pHraw / 4) ;
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
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
      vTaskDelay(100 / portTICK_PERIOD_MS);
      adc.setCompareChannels(ADS1115_COMP_0_3);
      adc.setVoltageRange_mV(ADS1115_RANGE_4096);
      adc.setConvRate(ADS1115_860_SPS);
      long cont=0;
      double sensorValue=0;
      while ( cont < ADS1115_MiddleCount){
       cont++;
        sensorValue = adc.getResult_mV()+sensorValue;
      }
      pHmV=sensorValue/cont;
      //pHmV=PhMediana.filtered(adc.getResult_mV());
    }
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
    bme.begin();
    AirTemp=bme.readTemperature();
    AirHum=bme.readHumidity();
    AirPress=bme.readPressure();
    }
  }
  }
#endif 