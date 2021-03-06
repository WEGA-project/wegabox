
// страница перезагрузки устройства http://[ip]/reset
void handleReset(){
  server.send(200, "text/plain",  "restart");
  delay(5000);
  ESP.restart();
  
}

// Основная страница http://[ip]
void handleRoot() {
  String httpstr="<meta http-equiv='refresh' content='10'>";
       httpstr += "HOSTNAME=" + String(HOSTNAME) + "<br>";
       httpstr += "Firmware=" + String(Firmware) + "<br>";
       if(RootTemp) { httpstr +=  "RootTemp=" + fFTS(RootTemp,3) + "<br>"; }
       if(AirTemp)  { httpstr +=  "AirTemp=" +  fFTS(AirTemp,3) + "<br>"; }
       if(AirHum)   { httpstr +=  "AirHum=" +   fFTS(AirHum,3) + "<br>"; }
       if(CO2)   { httpstr +=  "CO2=" +   fFTS(CO2,0) + "<br>"; }
       if(tVOC)   { httpstr +=  "tVOC=" +   fFTS(tVOC,0) + "<br>"; }
       if(hall)   { httpstr +=  "hall=" +   fFTS(hall,3) + "<br>"; }     
       if(pHmV)   { httpstr +=  "pHmV=" +   fFTS(pHmV,3) + "<br>"; }
       if(NTC)   { httpstr +=  "NTC=" +   fFTS(NTC,3) + "<br>"; }
       if(Ap)   { httpstr +=  "Ap=" +   fFTS(Ap,3) + "<br>"; }
       if(An)   { httpstr +=  "An=" +   fFTS(An,3) + "<br>"; }
       if(Dist)   { httpstr +=  "Dist=" +   fFTS(Dist,3) + "<br>"; }
       if(PR)   { httpstr +=  "PR=" +   fFTS(PR,3) + "<br>"; }
       if(AirPress)   { httpstr +=  "AirPress=" +   fFTS(AirPress,3) + "<br>"; }
       if(CPUTemp)   { httpstr +=  "CPUTemp=" +   fFTS(CPUTemp,3) + "<br>"; }

        if(wNTC >0 )  { httpstr +=  "wNTC=" +  fFTS(wNTC,3) + "<br>"; }
        if(wR2 >0 )   { httpstr +=  "wR2=" +   fFTS(wR2, 3) + "<br>"; }
        if(wEC >0 )   { httpstr +=  "wEC=" +   fFTS(wEC, 3) + "<br>"; }





  server.send(200, "text/html",  httpstr);
  }

// Страница статуса http://[ip]/status
void handleStatus()
{

  String statusstr = "status ok \n";

  statusstr += "Uptime=" + fFTS(millis() / 1000, 0) + "\n";
  statusstr += "EC Times=" + fFTS(float(t_EC / 1000), 3) + " msec\n";
  statusstr += "EC Freq=" + fFTS(f_EC, 3) + " Hz\n";

  if (wegareply != "")
  {
    statusstr += "WEGAAPI: " + wegareply + "\n";
    statusstr += "WEGA-API JSON STATUS: " + err_wegaapi_json + "\n";
    statusstr += "wNTC=" + fFTS(wNTC, 3) + " C\n";
    statusstr += "wR2=" + fFTS(wR2, 3) + " Omh\n";
    statusstr += "wEC=" + fFTS(wEC, 3) + " mS/cm\n";    
  }

  server.send(200, "text/plain", statusstr);
}