// В этом файле расположены функции выполняющиеся параллельно freeRTOC

void TaskOTA(void * parameters){
  for(;;){
    server.handleClient();
    ArduinoOTA.handle();
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void TaskWegaApi(void * parameters){
  for(;;){
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
    vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
    //delay (freqdb*1000); // Периодичность отправки данных в базу (в мс)

  }
}


// Измерение фоторезистора
#if c_PR == 1
void TaskPR(void * parameters){
  pinMode(PR_AnalogPort, INPUT);
  for(;;){
   float PR0=0;
   double prcount = 0;
   while (prcount < PR_MiddleCount){
    prcount++;
    PR0=analogRead(PR_AnalogPort)+PR0;
    vTaskDelay(1 / portTICK_PERIOD_MS);
   }
   PR=PR0/PR_MiddleCount;
   vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
  }
}
#endif //  c_PR

// Измерение термистора
#if c_NTC == 1
  void TaskNTC(void * parameters){
    pinMode(NTC_port, INPUT);
    for(;;){
    float NTC0=0;
    double ntccount = 0;
    while (ntccount < NTC_MiddleCount){
      ntccount++;
      NTC0=analogRead(NTC_port)+NTC0;
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    NTC=NTC0/NTC_MiddleCount;
    vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
    }
  }
#endif // c_NTC

// Измерение ЕС
#if c_EC == 1
void TaskEC(void * parameters){
  for(;;){

if ( USwork==false ){

    ECwork=true;
     float Ap0 = 0;
    float An0 = 0;
    double eccount = 0;

    pinMode(EC_AnalogPort, INPUT);

  while (eccount < EC_MiddleCount and OtaStart != true){
    pinMode(EC_DigitalPort1, OUTPUT);
    pinMode(EC_DigitalPort2, OUTPUT);
    digitalWrite(EC_DigitalPort1, LOW);
    digitalWrite(EC_DigitalPort2, LOW);

    eccount++;
      digitalWrite(EC_DigitalPort1, HIGH);
      digitalWrite(EC_DigitalPort2, LOW);
        Ap0 = analogRead(EC_AnalogPort) + Ap0;
                
      digitalWrite(EC_DigitalPort2, HIGH);
      digitalWrite(EC_DigitalPort1, LOW);
        An0 = analogRead(EC_AnalogPort) + An0;
      
      
  
    //vTaskDelay(1 / portTICK_PERIOD_MS); 

    pinMode(EC_DigitalPort1, INPUT);
    pinMode(EC_DigitalPort2, INPUT);

  }
    pinMode(EC_AnalogPort, INPUT);    


    Ap = Ap0 / eccount;
    An = An0 / eccount;
    ECwork=false;

}
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  //server.handleClient();

  }
}
#endif // c_EC

#if c_US025 == 1
void TaskUS(void * parameters) {
  for(;;){
    

    //     float Dist0;
    // Dist0=DstMediana.filtered(distanceSensor.measureDistanceCm(25)*100 );
    // Dist=Dist0/100;
    // vTaskDelay(2000 / portTICK_PERIOD_MS);

    // vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
    // //Dist=us(US_ECHO,US_TRIG,25,US_MiddleCount);


    //Dist=distanceSensor.measureDistanceCm(25);
if (ECwork==false){
  USwork=true;
    long cont=0;
    double sensorValue=0;

    
      
    while ( cont < US_MiddleCount){
      
      float dist0=distanceSensor.measureDistanceCm(25);
      if(dist0 != -1 ) {
        sensorValue=dist0+sensorValue;
        cont++;  
        delay (5); 
         }
    } 
    
    
    Dist=sensorValue/cont;

    USwork=false;
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS);


    // //Dist=Dstkalman.filtered( us(US_ECHO,US_TRIG,25,US_MiddleCount) );
    // // if(millis()<60000){
    // //   //Dstkalman.setParameters(1,1);
    // //   us(US_ECHO,US_TRIG,25,US_MiddleCount);
    // //   }
    // // else{
    // //   //Dstkalman.setParameters(1,0.01);
    // //   Dist=Dstkalman.filtered( us(US_ECHO,US_TRIG,25,US_MiddleCount) );
    // //   }
  }
}
#endif // c_US025

#if c_hall == 1
void TaskHall(void * parameters) {
  for(;;){
    long n=0;
    double sensorValue=0;
    while ( n < 100){
      n++;
      sensorValue = hallRead()+sensorValue;
      }
    vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
    
    hall=sensorValue/n;
  }
}
#endif //c_hall

#if c_CPUTEMP == 1
void TaskCPUtemp(void * parameters) {
  for(;;){
    //CPUTemp=CpuTempKalman.filtered( ( temprature_sens_read()-32) * (5/9) );
     int CPUTemp0 = temprature_sens_read();
     //if (CPUTemp0 != 128 ) CPUTemp=CpuTempKalman.filtered( ( CPUTemp0 - 32 )/1.8 );
     CPUTemp=CpuTempKalman.filtered( ( CPUTemp0 - 32 )/1.8 );
    vTaskDelay(100 / portTICK_PERIOD_MS);
    rtc_wdt_feed();
  }
}
#endif //c_CPUTEMP


#if c_AHT10 == 1
  void TaskAHT10(void * parameters) {
  for(;;){
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
  #endif // c_MCP3421



#if c_DS18B20 == 1
void TaskDS18B20(void * parameters) {
  for(;;){
    vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
sens18b20.begin();
sens18b20.requestTemperatures();
float ds0=sens18b20.getTempCByIndex(0);
if(ds0 != -127 and ds0 !=85) RootTemp=ds0; 
  }
}
#endif

#if c_ADS1115 == 1
  void TaskADS1115(void * parameters) {
    for(;;){
      vTaskDelay(10000 / portTICK_PERIOD_MS);
      adc.setCompareChannels(ADS1115_COMP_0_3);
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
#endif // c_ADS1115



#if c_AM2320 == 1
void TaskAHT10(void * parameters) {
for(;;){
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
#endif

#if c_BME280 == 1
  void TaskBME280(void * parameters) {
  for(;;){  
    vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
    bme.begin();
    AirTemp=bme.readTemperature();
    AirHum=bme.readHumidity();
    AirPress=bme.readPressure();
    }
  }
#endif 