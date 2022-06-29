void handleSettings()
{

  syslog_ng("WEB /settings");

  String h = "<!DOCTYPE html> <html>\n";
  h += "HOSTNAME=" + String(HOSTNAME) + "<br>\n";
  h += "Firmware=" + String(Firmware) + "<br>\n";

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