void handleSettings()
{

  syslog_ng("WEB /settings");

  String h = "<!DOCTYPE html> <html>\n";

  h += " <table>\n";
  h += "<tr><td>HOSTNAME<td>" + String(HOSTNAME) + "</tr>\n";
  h += "<tr><td>Firmware<td>" + String(Firmware) + "</tr>\n";
  h += "<tr><td>AirHum<td>" + fFTS(AirHum, 3) + "%</tr>\n";
  h += "<tr><td>RootTemp<td>" + fFTS(RootTemp, 3) + "</tr>\n";
  h += "<tr><td>AirTemp<td>" + fFTS(AirTemp, 3) + "</tr>\n";
  h += "<tr><td>AirTemp-RootTemp<td>" + fFTS(AirTemp - RootTemp, 3) + "</tr>\n";
  h += "<tr><td>Dist<td>" + fFTS(Dist, 3) + "</tr>\n";
  h += "<tr><td>Light Sensor<td>" + fFTS(PR, 3) + "</tr>\n";
  h += "<tr><td>EC<td>" + fFTS(wEC, 3) + "mS/cm</tr>\n";
  h += "<tr><td>pH<td>" + fFTS(wpH, 3) + "</tr>\n";
  h += "<tr><td>PWD1<td>" + String(PWD1) + "</tr>\n";
  h += "<tr><td>PWD2<td>" + String(PWD2) + "</tr>\n";
  h += "</table>\n";

#include <web/SettingsRootPomp.h>
#include <web/SettingsECCorr.h>
#include <web/SettingsPompNight.h>
  //#include <web/RootPomp.html>

  server.send(200, "text/html", h);
}

void handleSettingsPomps()
{
  String h = "<!DOCTYPE html> <html>\n";
#include <web/SettingsPoms.h>
  server.send(200, "text/html", h);
}