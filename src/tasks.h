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
    if(RootTemp) httpstr +=  "&RootTemp=" + fFTS(RootTemp,3);
    if(AirTemp) httpstr +=  "&AirTemp=" +fFTS(AirTemp, 3);
    if(AirHum) httpstr +=  "&AirHum=" +fFTS(AirHum, 3);
    if(hall) httpstr +=  "&hall=" +fFTS(hall, 3);
    if(pHmV) httpstr +=  "&pHmV=" +fFTS(pHmV, 4);
    if(pHraw) httpstr +=  "&pHraw=" +fFTS(pHraw, 4);
    if(CO2) httpstr +=  "&CO2=" +fFTS(CO2, 0);
    if(tVOC) httpstr +=  "&tVOC=" +fFTS(tVOC, 0);
    if(NTC) httpstr +=  "&NTC=" +fFTS(NTC, 3);
    if(Ap) httpstr +=  "&Ap=" +fFTS(Ap, 3);
    if(An) httpstr +=  "&An=" +fFTS(An, 3);
    if(Dist) httpstr +=  "&Dist=" +fFTS(Dist, 3);
    if(PR) httpstr +=  "&PR=" +fFTS(PR, 3);

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


void TaskEC(void * parameters){
  for(;;){
  
  #if c_EC == 1
    float Ap0 = 0;
    float An0 = 0;
    double eccount = 0;

    #if c_NTC == 1
      float NTC0=0;
    #endif // c_NTC 

    pinMode(EC_AnalogPort, INPUT);
    pinMode(NTC_port, INPUT);
    pinMode(EC_DigitalPort1, OUTPUT);
    pinMode(EC_DigitalPort2, OUTPUT);

    while (eccount < EC_MiddleCount){
      eccount++;
      digitalWrite(EC_DigitalPort1, HIGH);
      Ap0 = analogRead(EC_AnalogPort) + Ap0;
      digitalWrite(EC_DigitalPort1, LOW);
      
      digitalWrite(EC_DigitalPort2, HIGH);
      An0 = analogRead(EC_AnalogPort) + An0;
      digitalWrite(EC_DigitalPort2, LOW);

    ArduinoOTA.handle();
    vTaskDelay(1 / portTICK_PERIOD_MS); 

    #if c_NTC == 1
      NTC0=analogRead(NTC_port)+NTC0;
    #endif // c_NTC 

    }
    
    pinMode(EC_DigitalPort1, INPUT);
    pinMode(EC_DigitalPort2, INPUT);
    pinMode(EC_AnalogPort, INPUT);

    Ap = Ap0 / eccount;
    An = An0 / eccount;

    #if c_NTC == 1
      NTC = NTC0 / eccount;
    #endif // c_NTC

    vTaskDelay(200 / portTICK_PERIOD_MS);
    server.handleClient();
  #endif // c_EC
  }

}

#if c_US025 == 1
void TaskDist(void * parameters){
  float temp=25;
  long count, startmic, microssum, endmicros;

  for(;;){
    server.handleClient();
    ArduinoOTA.handle();
    vTaskDelay(200 / portTICK_PERIOD_MS);

    
    
    count=0;
    microssum=0;

    while (count < US_MiddleCount) {
      count++;
      ArduinoOTA.handle();
      pinMode(US_TRIG, OUTPUT );
      pinMode(US_ECHO, INPUT);
      digitalWrite(US_TRIG,1);
      delayMicroseconds(10);
      digitalWrite(US_TRIG,0);
      long n=0;
      long limit=10000;

      while (n<limit){
        n++;
        if(digitalRead(US_ECHO) == 1)  
        {
          startmic=micros();
          long z=0;
          while (digitalRead(US_ECHO) == 1 and z<20000) {
            z++;
            endmicros=micros();
          }
          n = limit;
          vTaskDelay(200 / portTICK_PERIOD_MS);
          //delay(200);
        
        }
        
      }

      microssum=microssum+(endmicros-startmic);
    }

    float vSound=20.046796*sqrt(273.15+temp);
    float DistCm = (vSound/10000)*((float(microssum)/count)/2);
    
    if (DistCm > 0 and DistCm < 200 ) Dist=DstGAB.filtered(DistCm); // В сантиметрах слать только реальные значения от 0 до 2 метров
    
  }
} // TaskDist
#endif // c_US025