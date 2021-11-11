void TaskWegaApi(void * parameters){
    for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
    // Отправка данных на WEGA-API 
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
    wegareply = http.getString();


// Получение данных от WEGA-API
DynamicJsonDocument doc(2048);
DeserializationError error = deserializeJson(doc, wegareply);

if (error){err_wegaapi_json = 0;}else{err_wegaapi_json = 1;}
// {
//   err_wegaapi_json = false;
  //dt = doc["dt"];
  EC_R1 = doc["EC_R1"];
  EC_R2_p1 = doc["EC_R2_p1"];
  EC_R2_p2 = doc["EC_R2_p2"];
  //String A1 = doc["A1"];
  //long    uptime = doc["uptime"]; // "51"
  //float    param1 = doc["param1"]; // 73

  //statusstr += "param1="+ fFTS(param1,3)+ "\n";
  //statusstr += "dt=" + dt + "\n";
  //statusstr += "A1=" + A1 + "\n";
//}

    http.end();

      if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect(true);
        WiFi.begin(ssid, password);  }
    }
    vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
    //delay (freqdb*1000); // Периодичность отправки данных в базу (в мс)

  
  }
}







