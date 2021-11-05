// В этом файле расположены функции выполняющиеся параллельно freeRTOC

void TaskOTA(void * parameters){
  for(;;){
    server.handleClient();
    ArduinoOTA.handle();
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}

void TaskWegaApi(void * parameters){
    for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
    // Sending to WEGA-API 
    WiFiClient client;
    HTTPClient http;

    String httpstr=wegaapi;
    httpstr +=  "?db=" + wegadb;
    httpstr +=  "&auth=" + wegaauth;
    httpstr +=  "&uptime=" +fFTS(millis()/1000, 0);
    if(RootTemp) httpstr +=  "&" + db_RootTemp + "=" +fFTS(RootTemp,3);
    if(AirTemp) httpstr +=  "&" + db_AirTemp + "=" +fFTS(AirTemp, 3);
    if(AirHum) httpstr +=  "&" + db_AirHum + "=" +fFTS(AirHum, 3);
    if(hall) httpstr +=  "&" + db_hall + "=" +fFTS(hall, 3);
    if(pHmV) httpstr +=  "&" + db_pHmV + "=" +fFTS(pHmV, 4);
    if(pHraw) httpstr +=  "&" + db_pHraw + "=" +fFTS(pHraw, 4);
    if(CO2) httpstr +=  "&" + db_CO2 + "=" +fFTS(CO2, 0);
    if(tVOC) httpstr +=  "&" + db_tVOC + "=" +fFTS(tVOC, 0);
    if(NTC) httpstr +=  "&" + db_NTC + "=" +fFTS(NTC, 3);
    if(Ap) httpstr +=  "&" + db_Ap + "=" +fFTS(Ap, 3);
    if(An) httpstr +=  "&" + db_An + "=" +fFTS(An, 3);
    if(Dist) httpstr +=  "&" + db_Dist + "=" +fFTS(Dist, 3);
    if(PR) httpstr +=  "&" + db_PR + "=" +fFTS(PR, 3);
    if(AirPress) httpstr +=  "&" + db_AirPress + "=" +fFTS(AirPress, 3);
    if(CPUTemp) httpstr +=  "&" + db_CPUTemp + "=" +fFTS(CPUTemp, 3);

    if(t_EC) httpstr +=  "&t_EC=" +fFTS(t_EC, 3);
    if(f_EC) httpstr +=  "&f_EC=" +fFTS(f_EC, 3);
    
    http.begin(client, httpstr);
    http.GET();
    http.end();

      if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect(true);
        WiFi.begin(ssid, password);  }
    }
    vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
    //delay (freqdb*1000); // Периодичность отправки данных в базу (в мс)

  }
}


// Измерение фоторезистора
#if c_PR == 1
void TaskPR(void * parameters){
  pinMode(PR_AnalogPort, INPUT);
  float PR0;

  for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
   
   
    PR0=PRGAB.filtered (analogRead(PR_AnalogPort));
   
  
   vTaskDelay(100 / portTICK_PERIOD_MS);
   if (millis()>60000){
     PRGAB.setParameters(0.0001,1,1);
     PR=PR0; 
     }
  }
}
}
#endif //  c_PR

// Измерение ЕС и NTC
void TaskEC(void *parameters)
{
for(;;){



    if (OtaStart == true) {vTaskDelete( NULL );}else{

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
    adc1_config_channel_atten(EC_AnalogPort,ADC_ATTEN_DB_11);

      t_EC0=micros();
      digitalWrite(EC_DigitalPort1, LOW);
      digitalWrite(EC_DigitalPort2, LOW);
	
static portMUX_TYPE my_mutex = portMUX_INITIALIZER_UNLOCKED;
  rtc_wdt_protect_off();
  rtc_wdt_disable();
  disableCore0WDT();
  disableLoopWDT();
  vPortCPUInitializeMutex(&my_mutex);
      for (long i=0;i<EC_MiddleCount and OtaStart != true;i++){

	//vPortCPUInitializeMutex(&my_mutex);

      pinMode(EC_DigitalPort1, OUTPUT);
      pinMode(EC_DigitalPort2, OUTPUT);
      digitalWrite(EC_DigitalPort1, LOW);
      digitalWrite(EC_DigitalPort2, LOW);
      
	portENTER_CRITICAL(&my_mutex);
          digitalWrite(EC_DigitalPort1, HIGH); 
          Ap0 =adc1_get_raw(EC_AnalogPort)+Ap0;
          //delayMicroseconds(3);
          digitalWrite(EC_DigitalPort1, LOW);

          digitalWrite(EC_DigitalPort2, HIGH);
          An0 =adc1_get_raw(EC_AnalogPort)+An0;
          //delayMicroseconds(3);
          digitalWrite(EC_DigitalPort2, LOW);
portEXIT_CRITICAL(&my_mutex);


      pinMode(EC_DigitalPort1, INPUT);
      pinMode(EC_DigitalPort2, INPUT);

//vPortCPUReleaseMutex(&my_mutex);
      //delayMicroseconds(5000);
      
      }
      
      t_EC=micros()-t_EC0;
esp_task_wdt_reset();

rtc_wdt_protect_on();
  rtc_wdt_enable();
  enableCore0WDT();
   enableLoopWDT();

      float Mid_Ap0=float(Ap0)/EC_MiddleCount;
      float Mid_An0=float(An0)/EC_MiddleCount;

      pinMode(EC_DigitalPort1, INPUT);
      pinMode(EC_DigitalPort2, INPUT);
      ECwork = false;
      f_EC = EC_MiddleCount /(float(t_EC)/1000000);

      // float GAB_Ap0=ApGAB.filtered(Mid_Ap0);
      // float GAB_An0=AnGAB.filtered(Mid_An0);

      // if ( abs(GAB_Ap0-Mid_Ap0)/Mid_Ap0 < 0.05)
      // {
      //   ApGAB.setParameters(0.05, 1, 1);
      //   AnGAB.setParameters(0.05, 1, 1);
      //   Ap = GAB_Ap0;
      //   An = GAB_An0;

      // }
      // else
      // {
      //   ApGAB.setParameters(10, 1, 1);
      //   AnGAB.setParameters(10, 1, 1); 
      //   Ap = Mid_Ap0;
      //   An = Mid_An0;
      // }
        // Ap = float(Ap0)/EC_MiddleCount;
        // An = float(An0)/EC_MiddleCount;
        if(Mid_Ap0 < 4095) Ap = Mid_Ap0;
        if(Mid_An0 > 0) An = Mid_An0;
       
      vTaskDelay(100 / portTICK_PERIOD_MS);


#endif // c_EC

// Измерение термистора
#if c_NTC == 1
      adc1_config_channel_atten(NTC_port,ADC_ATTEN_DB_11);
      
      unsigned long NTC0=0;
      s=0;
      while(s<NTC_MiddleCount and OtaStart != true ){
        s++;        
        NTC0 = adc1_get_raw(NTC_port)+NTC0 ;
        //esp_task_wdt_reset();
      }

      //NTCGAB.filtered(NTC0/s);
      // if (millis() > 180000 and OtaStart != true)
      // {
      //   NTCGAB.setParameters(1, 1, 1);
      //   NTC = NTCGAB.filtered(float(NTC0)/s);
      //   NTC = float(NTC0)/s;
      // }
      NTC = float(NTC0)/s;
      vTaskDelay(20000 / portTICK_PERIOD_MS);
#endif // c_NTC      
    }
  

  
  
  }
}

// // Измерение термистора
// #if c_NTC == 1
//   void TaskNTC(void * parameters){
//     pinMode(NTC_port, INPUT);
//     float NTC0;
//     for(;;){
//       if (OtaStart == true) {delay (120000);}else{
//         if (ECwork == false){
//           NTC0=NTCGAB.filtered (analogRead(NTC_port));
          
//           if (millis()>60000){
//             NTCGAB.setParameters(0.0001,1,1);
//             NTC=NTC0;
//           }
//         vTaskDelay(1 / portTICK_PERIOD_MS);
//         }
//       }
//     }
//   }
// #endif // c_NTC

#if c_US025 == 1
void TaskUS(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
    {
      vTaskDelete(NULL);
    }
    else
    {
      float us = distanceSensor.measureDistanceCm(25);
      if (us != -1)
      {
        float dst0 = 0;
        long dcnt = 0;
        for (long i = 0; i < US_MiddleCount; i++)
        {
          us = distanceSensor.measureDistanceCm(25);
          if (us != -1)
          {
            dst0 = dst0 + us;
            dcnt++;
          }
          vTaskDelay(50 / portTICK_PERIOD_MS);
        }
        Dist = dst0 / dcnt;
      }
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS);
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
  void TaskAHT10(void * parameters) {
  for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
   readStatus = myAHT10.readRawData();
  
  if (readStatus != AHT10_ERROR)
  {
   float AirTemp0=myAHT10.readTemperature();
   float AirHum0=myAHT10.readHumidity();
   if (AirTemp0 != 255 ) AirTemp=AirTemp0;
   if (AirHum0 != 255 ) AirHum=AirHum0;
  }
  else
  {
    myAHT10.softReset();
    delay(50);
    myAHT10.begin();
    myAHT10.setNormalMode();
  }

   vTaskDelay(2000 / portTICK_PERIOD_MS);
    
    }
  }
}
#endif // c_AHT10    
    

  #if c_CCS811 == 1
    void TaskCCS811(void * parameters) {
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
    }
  }
} 
  #endif



    #if c_MCP3421 == 1
void TaskMCP3421(void * parameters) {
  for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
    vTaskDelay(100 / portTICK_PERIOD_MS);

    long value = 0;
    MCP342x::Config status;
    // Initiate a conversion; convertAndRead() will wait until it can be read
    uint8_t err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot,MCP342x::resolution18, MCP342x::gain4,1000000, value, status);
    if (err) {
      Serial.print("Convert error: ");
      Serial.println(err);
    }
    else {
      pHraw=PhMediana.filtered(value);  // Медианная фильтрация удаляет резкие выбросы показаний
      if (millis() < 60000){            // Игнорит ошибку фильтра на старте системы первые 60 сек. 
        PhGAB.setParameters(10,10,10);
        PhGAB.filtered(pHraw);
        //pHmV=4096/pow(2,18)*pHraw/1;
      }else{
        PhGAB.setParameters(0.001, 200, 1);
        pHmV=4096/pow(2,18)*PhGAB.filtered(pHraw)/4;
      }
    }
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
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      //sens18b20.begin();
      //vTaskDelay(500 / portTICK_PERIOD_MS);
      sens18b20.requestTemperatures();
      float ds0 = sens18b20.getTempCByIndex(0);
      if (ds0 != -127 and ds0 != 85)
        RootTemp = ds0;
    }
  }
}
#endif

#if c_ADS1115 == 1
  void TaskADS1115(void * parameters) {
    for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
      vTaskDelay(10000 / portTICK_PERIOD_MS);
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

    }
  }
}
#endif // c_ADS1115



#if c_AM2320 == 1
void TaskAHT10(void * parameters) {
for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
  vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
int status = AM2320.read();
switch (status)
{
  case AM232X_OK:
    AirHum=AM2320.getHumidity();
    AirTemp=AM2320.getTemperature();
    break;
  default:
    Serial.println(status);
  break;
    }
  }
}
}
#endif

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