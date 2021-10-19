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
  long s;
for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{

#if c_EC == 1
      pinMode(EC_AnalogPort, INPUT);
      double An0 = 0;
      double Ap0 = 0;

      pinMode(EC_DigitalPort1, OUTPUT);
      pinMode(EC_DigitalPort2, OUTPUT);
      ECwork = true;
      s=0;
      while(s<400){
        s++;
        digitalWrite(EC_DigitalPort1, HIGH);
        Ap0 = analogRead(EC_AnalogPort)+Ap0;
        digitalWrite(EC_DigitalPort1, LOW);

        digitalWrite(EC_DigitalPort2, HIGH);
        An0 = analogRead(EC_AnalogPort)+An0;
        digitalWrite(EC_DigitalPort2, LOW);
      }

      pinMode(EC_DigitalPort1, INPUT);
      pinMode(EC_DigitalPort2, INPUT);
      ECwork = false;
      

      ApGAB.filtered(Ap0/s);
      AnGAB.filtered(An0/s);

      if (millis() > 180000)
      {
        ApGAB.setParameters(0.0001, 1, 1);
        AnGAB.setParameters(0.0001, 1, 1);
        Ap = ApGAB.filtered(Ap0/s);
        An = AnGAB.filtered(An0/s);
      }
      vTaskDelay(30 / portTICK_PERIOD_MS);
#endif // c_EC


#if c_NTC == 1
      pinMode(NTC_port, INPUT);
      long NTC0=0;
      s=0;
      while(s<128){
        s++;
        NTC0 = NTCMed.filtered(analogRead(NTC_port))+NTC0 ;
      }

      NTCGAB.filtered(NTC0/s);
      if (millis() > 180000)
      {
        NTCGAB.setParameters(0.001, 1, 1);
        NTC = NTCGAB.filtered(NTC0/s);
      }
      //vTaskDelay(5 / portTICK_PERIOD_MS);
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
void TaskUS(void * parameters) {
  for(;;){
    float dst0;
    if (OtaStart == true) {vTaskDelete( NULL );}else{
      float us=distanceSensor.measureDistanceCm(25);
      if(us != -1 ) {    
        dst0=DstGAB.filtered(us);  
        delay (50); 
        if (millis()>120000){
          DstGAB.setParameters(0.00001,1,1);
          Dist=dst0; 
        }
      }
    }
   }
    vTaskDelay(10 / portTICK_PERIOD_MS);
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

   vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
    
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
void TaskDS18B20(void * parameters) {
  for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
    vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
sens18b20.begin();
sens18b20.requestTemperatures();
float ds0=sens18b20.getTempCByIndex(0);
if(ds0 != -127 and ds0 !=85) RootTemp=ds0; 
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

void TaskECclean(void * parameters) {
  for(;;){
    vTaskDelay(1000*60*60 / portTICK_PERIOD_MS); 
      pinMode(EC_DigitalPort1, OUTPUT);
      pinMode(EC_DigitalPort2, OUTPUT);
     
      long s=0;
      while(s<4000000){
        s++;
        digitalWrite(EC_DigitalPort1, HIGH);
        digitalWrite(EC_DigitalPort1, LOW);

        digitalWrite(EC_DigitalPort2, HIGH);
        digitalWrite(EC_DigitalPort2, LOW);
      }
     
      pinMode(EC_DigitalPort1, INPUT);
      pinMode(EC_DigitalPort2, INPUT);
     
  }
}