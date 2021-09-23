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

    http.begin(client, httpstr);
    http.GET();
    http.end();

      if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect(true);
        WiFi.begin(ssid, password);  }
    ArduinoOTA.handle();   
    delay (20000); // Периодичность отправки данных в базу (в мс)
  }
  
}

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
   vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
#endif //  c_PR


void TaskEC(void * parameters){
  for(;;){
  
  #if c_EC == 1
    float Ap0 = 0;
    float An0 = 0;
    double eccount = 0;
    pinMode(EC_DigitalPort1, OUTPUT);
    pinMode(EC_DigitalPort2, OUTPUT);
    pinMode(EC_AnalogPort, INPUT);
  #endif // c_EC

  #if c_NTC == 1
    float NTC0=0;
    pinMode(NTC_port, INPUT);
  #endif // c_NTC 

  while (eccount < EC_MiddleCount){
    eccount++;
    #if c_EC == 1
      digitalWrite(EC_DigitalPort1, HIGH);
      Ap0 = analogRead(EC_AnalogPort) + Ap0;
      digitalWrite(EC_DigitalPort1, LOW);
      
      digitalWrite(EC_DigitalPort2, HIGH);
      An0 = analogRead(EC_AnalogPort) + An0;
      digitalWrite(EC_DigitalPort2, LOW);
    #endif // c_EC

  ArduinoOTA.handle();
  vTaskDelay(1 / portTICK_PERIOD_MS); 

  #if c_NTC == 1
    NTC0=analogRead(NTC_port)+NTC0;
  #endif // c_NTC 
  }
    
  #if c_EC == 1
    pinMode(EC_DigitalPort1, INPUT);
    pinMode(EC_DigitalPort2, INPUT);
    pinMode(EC_AnalogPort, INPUT);

    Ap = Ap0 / eccount;
    An = An0 / eccount;
  #endif // c_EC

  #if c_NTC == 1
    NTC = NTC0 / eccount;
  #endif // c_NTC

  vTaskDelay(200 / portTICK_PERIOD_MS);
  server.handleClient();

  }
}

#if c_US025 == 1
void TaskUS(void * parameters) {
  for(;;){
    long ndist=0;
    float Dist0=0;
    while (ndist < 15000){ // примерно 15 тыс измерений в минуту
      ndist++;
      Dist0=distanceSensor.measureDistanceCm(25)+Dist0;
      vTaskDelay(5 / portTICK_PERIOD_MS);
    }
    Dist=Dist0/ndist;
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
    vTaskDelay(1500 / portTICK_PERIOD_MS);
    hall=sensorValue/n;
  }
}
#endif