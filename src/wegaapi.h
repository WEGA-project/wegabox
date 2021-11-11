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
DynamicJsonDocument doc(4096);
DeserializationError error = deserializeJson(doc, wegareply);
if (error){err_wegaapi_json = error.f_str();}

// Получение переменных для терморезистора
String tR_type;
float tR_DAC,tR_B,tR_val_korr;

tR_DAC=doc["tR_DAC"];
tR_B=doc["tR_B"];
tR_val_korr=doc["tR_val_korr"];

// Вычисление температуры терморезистора
if (tR_type = "direct"){
float r=log((-NTC+tR_DAC)/NTC);
wNTC= (tR_B*25-r*237.15*25-r*pow(237.15,2))/(tR_B+r*25+r*237.15)+tR_val_korr;
}
// Вычисление ЕС
// Вычисление сопротивлений
float A1,A2,R1,Rx1,Rx2,Dr,R2p,R2n;
A1=Ap;
A2=An;
R1=doc["EC_R1"];
Rx1=doc["EC_Rx1"];
Rx2=doc["EC_Rx2"];
Dr=doc["Dr"];

R2p=(((-A2*R1-A2*Rx1+R1*Dr+Rx1*Dr)/A2));
R2n=(-(-A1*R1-A1*Rx2+Rx2*Dr)/(-A1+Dr));
wR2=(R2p+R2n)/2;
// Расчет функции калибровки
float ec1=doc["EC_val_p1"];
float ec2=doc["EC_val_p2"];
float ex1=doc["EC_R2_p1"];
float ex2=doc["EC_R2_p2"];
float eckorr=doc["EC_val_korr"];
float kt=doc["EC_kT"];
float eb=(-log10(ec1/ec2))/(log10(ex2/ex1));
float ea=pow(ex1,(-eb))*ec1;
float ec=ea*pow(wR2,eb);
wEC=ec/(1+kt*(wNTC-25))+eckorr;



    http.end();

      if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect(true);
        WiFi.begin(ssid, password);  }
    }
    vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
    //delay (freqdb*1000); // Периодичность отправки данных в базу (в мс)

  
  }
}







