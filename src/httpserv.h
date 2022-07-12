
// страница перезагрузки устройства http://[ip]/reset
void handleReset()
{
  syslog_ng("WEB /reset");
  server.send(200, "text/plain", "restart");
  delay(1000);
  ESP.restart();
}

// index
void handleRoot()
{
  syslog_ng("WEB /root");
  String s = "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>";

  s += "<frameset cols='20%,*'>";
  s += "  <frame src='menu' name='MENU'>";
  s += "  <frame src='main' name='CONTENT'>";
  s += "</frameset>";
  server.send(200, "text/html", s);
}

// MENU
void handleMenu()
{

  String s = "<body>";
  s += "<u1>";
  s += "<li><a href='main' target='CONTENT'>main </a></li>";
  s += "<li><a href='status' target='CONTENT'>wega-server api</a></li>";
  s += "<li><a href='settings' target='CONTENT'>Settings</a></li>";
  s += "<li><a href='SettingsPomps' target='CONTENT'>SettingsPomps</a></li>";
  s += "</ul>";
  s += "<u1>";
  s += "<li><a href='reset' target='CONTENT'>REBOOT</a></li>";
  s += "</ul>";
  s += "</body>";

  server.send(200, "text/html", s);
}

// Основная страница
void handleMain()
{
  String httpstr = "<meta http-equiv='refresh' content='10'>";
  httpstr += "HOSTNAME=" + String(HOSTNAME) + "<br>";
  httpstr += "Firmware=" + String(Firmware) + "<br>";
  httpstr += "Uptime=" + fFTS(millis() / 1000, 0) + " sec <br>";

  if (RootTemp)
  {
    httpstr += "RootTemp=" + fFTS(RootTemp, 3) + "<br>";
  }
  if (AirTemp)
  {
    httpstr += "AirTemp=" + fFTS(AirTemp, 3) + "<br>";
  }
  if (AirHum)
  {
    httpstr += "AirHum=" + fFTS(AirHum, 3) + "<br>";
  }
  if (CO2)
  {
    httpstr += "CO2=" + fFTS(CO2, 0) + "<br>";
  }
  if (tVOC)
  {
    httpstr += "tVOC=" + fFTS(tVOC, 0) + "<br>";
  }
  if (hall)
  {
    httpstr += "hall=" + fFTS(hall, 3) + "<br>";
  }
  if (pHmV)
  {
    httpstr += "pHmV=" + fFTS(pHmV, 3) + "<br>";
  }
  if (NTC)
  {
    httpstr += "NTC=" + fFTS(NTC, 3) + "<br>";
  }
  if (Ap)
  {
    httpstr += "Ap=" + fFTS(Ap, 3) + "<br>";
  }
  if (An)
  {
    httpstr += "An=" + fFTS(An, 3) + "<br>";
  }
  if (Dist)
  {
    httpstr += "Dist=" + fFTS(Dist, 3) + "<br>";
  }
  if (PR != -1)
  {
    httpstr += "PR=" + fFTS(PR, 3) + "<br>";
  }
  if (AirPress)
  {
    httpstr += "AirPress=" + fFTS(AirPress, 3) + "<br>";
  }
  if (CPUTemp)
  {
    httpstr += "CPUTemp=" + fFTS(CPUTemp, 3) + "<br>";
  }

  if (wNTC > 0)
  {
    httpstr += "wNTC=" + fFTS(wNTC, 3) + "<br>";
  }
  if (wR2 > 0)
  {
    httpstr += "wR2=" + fFTS(wR2, 3) + "<br>";
  }
  if (wEC > 0)
  {
    httpstr += "wEC=" + fFTS(wEC, 3) + "<br>";
  }

  server.send(200, "text/html", httpstr);
}

// Страница статуса http://[ip]/status
void handleStatus()
{
  syslog_ng("WEB /status");
  String statusstr = "status ok \n";

  statusstr += "Uptime=" + fFTS(millis() / 1000, 0) + "\n";
  statusstr += "Last Time Reset_reason CPU 0:" + Reset_reason0 + "\n";
  statusstr += "Last Time Reset_reason CPU 1:" + Reset_reason1 + "\n";
  // statusstr += "EC Times=" + fFTS(float(t_EC / 1000), 3) + " msec\n";
  // statusstr += "EC Freq=" + fFTS(f_EC, 3) + " Hz\n";

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

// страница PWD
void handlePWD()
{
#if c_MCP23017 == 1
// mcp.pinMode(DRV1_A, OUTPUT);
// mcp.digitalWrite(DRV1_A, HIGH);

// pwd = server.arg("pwd").toInt();
// syslog_ng("PWD set:" + fFTS(pwd, 0));

// int freq = server.arg("freq").toInt();
// syslog_ng("PWD freq set:" + fFTS(freq, 0));

// // задаём свойства ШИМ-сигнала
// //const int freq = 45000;
// const int ledChannel = 0;
// const int resolution = 8;
// ledcSetup(ledChannel, freq, resolution);
// ledcAttachPin(PWD2, ledChannel);
// // ledcWrite(ledChannel, 255);
// // delay(1000);
// ledcWrite(ledChannel, pwd);

// server.send(200, "text/plain", "pwd=" + fFTS(pwd, 0) + " freq=" + fFTS(freq, 0));
#endif // c_MCP23017
}

#include <web/settings.h>