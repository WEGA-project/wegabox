void handleSettings()
{

  syslog_ng("WEB /settings");

  String h = "<!DOCTYPE html> <html>\n";
  h += "HOSTNAME=" + String(HOSTNAME) + "<br>\n";
  h += "Firmware=" + String(Firmware) + "<br>\n";

  #include <web/RootPomp.h>
  #include <web/SettingsPoms.h>

  //#include <web/RootPomp.html>



  // h += fFTS(dataF_read,2) + "<br>\n";
 

  server.send(200, "text/html", h);
}
