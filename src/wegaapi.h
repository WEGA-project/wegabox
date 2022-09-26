void TaskWegaApi(void *parameters)
{
  http.setConnectTimeout(10000);
  http.setTimeout(10000);
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    syslog_ng("WegaApi loop. WIFI RSSI="+ String( WiFi.RSSI() ) );
    
        // Отправка данных на WEGA-API


    String httpstr = wegaapi;
    httpstr += "?db=" + wegadb;
    httpstr += "&auth=" + wegaauth;
    httpstr += "&uptime=" + fFTS(millis() / 1000, 0);
    if (RootTemp and !isnan(RootTemp))
      httpstr += "&" + db_RootTemp + "=" + fFTS(RootTemp, 3);
    if (AirTemp and !isnan(AirTemp))
      httpstr += "&" + db_AirTemp + "=" + fFTS(AirTemp, 3);
    if (AirHum and !isnan(AirHum))
      httpstr += "&" + db_AirHum + "=" + fFTS(AirHum, 3);
    if (hall and !isnan(hall))
      httpstr += "&" + db_hall + "=" + fFTS(hall, 3);
    if (pHmV and !isnan(pHmV))
      httpstr += "&" + db_pHmV + "=" + fFTS(pHmV, 4);
    if (pHraw and !isnan(pHraw))
      httpstr += "&" + db_pHraw + "=" + fFTS(pHraw, 4);
    if (CO2 and !isnan(CO2))
      httpstr += "&" + db_CO2 + "=" + fFTS(CO2, 0);
    if (tVOC and !isnan(tVOC))
      httpstr += "&" + db_tVOC + "=" + fFTS(tVOC, 0);

    if (NTC and !isnan(NTC))
      httpstr += "&" + db_NTC + "=" + fFTS(NTC, 3);
    if (Ap and !isnan(Ap))
      httpstr += "&" + db_Ap + "=" + fFTS(Ap, 3);
    if (An and !isnan(An))
      httpstr += "&" + db_An + "=" + fFTS(An, 3);
    if (Dist and !isnan(Dist))
      httpstr += "&" + db_Dist + "=" + fFTS(Dist, 3);
    if (DstRAW and !isnan(DstRAW))
      httpstr += "&DstRAW=" + fFTS(DstRAW, 3);
    if (PR != -1 and !isnan(PR))
      httpstr += "&" + db_PR + "=" + fFTS(PR, 3);
    if (AirPress and !isnan(AirPress))
      httpstr += "&" + db_AirPress + "=" + fFTS(AirPress, 3);
    if (CPUTemp and !isnan(CPUTemp))
      httpstr += "&" + db_CPUTemp + "=" + fFTS(CPUTemp, 3);

    // if(t_EC) httpstr +=  "&t_EC=" +fFTS(t_EC, 3);
    // if(f_EC) httpstr +=  "&f_EC=" +fFTS(f_EC, 3);

    if (wNTC and !isnan(wNTC) and isinf(wNTC))
      httpstr += "&wNTC=" + fFTS(wNTC, 3);
    if (wR2 and !isnan(wR2) and isinf(wR2))
      httpstr += "&wR2=" + fFTS(wR2, 3);
    if (wEC and !isnan(wEC) and isinf(wEC))
      httpstr += "&wEC=" + fFTS(wEC, 3);
    if (wpH and !isnan(wpH) and isinf(wpH))
      httpstr += "&wpH=" + fFTS(wpH, 3);


    if (eRAW and !isnan(eRAW) and isinf(eRAW))
      httpstr += "&eRAW=" + fFTS(eRAW, 3);
    
    if (readGPIO)
      httpstr += "&readGPIO=" + String(readGPIO);
    if (PWD1)
      httpstr += "&PWD1=" + String(PWD1);
    if (PWD2)
      httpstr += "&PWD2=" + String(PWD2);  
    if (ECStabOn) 
      httpstr += "&ECStabOn=" + String(ECStabOn); 

    httpstr += "&RSSI=" + String(WiFi.RSSI());

    http.begin(client, httpstr);
    //http.setTimeout(2000);
    http.GET();
    wegareply = http.getString();

    // Получение данных от WEGA-API
    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, wegareply);
    if (error)
    {
      err_wegaapi_json = error.f_str();
    }
    else
    {
      // Получение переменных для терморезистора
      String tR_type;
      float tR_DAC, tR_B, tR_val_korr;

      tR_DAC = doc["tR_DAC"];
      tR_B = doc["tR_B"];
      tR_val_korr = doc["tR_val_korr"];

      // Вычисление температуры терморезистора
      if (tR_type = "direct")
      {
        float r = log((-NTC + tR_DAC) / NTC);
        wNTC = (tR_B * 25 - r * 237.15 * 25 - r * pow(237.15, 2)) / (tR_B + r * 25 + r * 237.15) + tR_val_korr;
      }
      // Вычисление ЕС
      // Вычисление сопротивлений
      float A1, A2, R1, Rx1, Rx2, Dr, R2p, R2n;
      String A1name = doc["A1"];
      String A2name = doc["A2"];
      A1 = doc[A1name];
      A2 = doc[A2name];
      R1 = doc["EC_R1"];
      Rx1 = doc["EC_Rx1"];
      Rx2 = doc["EC_Rx2"];
      Dr = doc["Dr"];

      if (A1 and A2 and A1 < 4095 and A2 > 0)
      {
        R2p = (((-A2 * R1 - A2 * Rx1 + R1 * Dr + Rx1 * Dr) / A2));
        R2n = (-(-A1 * R1 - A1 * Rx2 + Rx2 * Dr) / (-A1 + Dr));
        wR2 = (R2p + R2n) / 2;

        // Расчет функции калибровки
        if (wR2 > 0)
        {

          float ec1 = doc["EC_val_p1"];
          float ec2 = doc["EC_val_p2"];
          float ex1 = doc["EC_R2_p1"];
          float ex2 = doc["EC_R2_p2"];
          float eckorr = doc["EC_val_korr"];
          float kt = doc["EC_kT"];
          float eb = (-log10(ec1 / ec2)) / (log10(ex2 / ex1));
          float ea = pow(ex1, (-eb)) * ec1;
          float ec = ea * pow(wR2, eb);
          wEC = ec / (1 + kt * (wNTC - 25)) + eckorr;
        }
      }
    }

    // Расчет pH
    if (pHmV)
    {

     float py1 = doc["pH_val_p1"];
     float py2 = doc["pH_val_p2"];
     float py3 = doc["pH_val_p3"];

     float px1 = doc["pH_raw_p1"];
     float px2 = doc["pH_raw_p2"];
     float px3 = doc["pH_raw_p3"];

     float pH_lkorr = doc["pH_lkorr"];
     
    float pa=-(-px1*py3+px1*py2-px3*py2+py3*px2+py1*px3-py1*px2)/(-pow(px1,2)*px3+pow(px1,2)*px2-px1*pow(px2,2)+px1*pow(px3,2)-pow(px3,2)*px2+px3*pow(px2,2) ); 
    float pb=(py3*pow(px2,2)-pow(px2,2)*py1+pow(px3,2)*py1+py2*pow(px1,2)-py3*pow(px1,2)-py2*pow(px3,2))/((-px3+px2)*(px2*px3-px2*px1+pow(px1,2)-px3*px1));
    float pc=(py3*pow(px1,2)*px2-py2*pow(px1,2)*px3-pow(px2,2)*px1*py3+pow(px3,2)*px1*py2+pow(px2,2)*py1*px3-pow(px3,2)*py1*px2 )/((-px3+px2)*(px2*px3-px2*px1+pow(px1,2)-px3*px1));

    wpH=pa*pow(pHmV,2) + pb* pHmV + pc+pH_lkorr;


    }
    
    http.end();

    if (WiFi.status() != WL_CONNECTED)
    {
      WiFi.disconnect(true);
      WiFi.begin(ssid, password);
    }
    vTaskDelay(freqdb * 1000 / portTICK_PERIOD_MS);
    //delay (freqdb*1000); // Периодичность отправки данных в базу (в мс)
  }
}
