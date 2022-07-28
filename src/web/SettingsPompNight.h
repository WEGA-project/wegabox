#if c_MCP23017 == 1

if (server.arg("PompNightEnable") != "")
  preferences.putInt("PompNightEnable", server.arg("PompNightEnable").toInt());
int PompNightEnable = preferences.getInt("PompNightEnable", 0);

if (server.arg("PompNightPomp") != "")
  preferences.putString("PompNightPomp", server.arg("PompNightPomp"));
String PompNightPomp = preferences.getString("PompNightPomp", "DRV1_A");

if (server.arg("MinLightLevel") != "")
  preferences.putInt("MinLightLevel", server.arg("MinLightLevel").toInt());
int MinLightLevel = preferences.getInt("MinLightLevel", 10);

h += "<form action='' method='POST' id='set'></form>\n";
h += " <table>\n";
h += "     <caption>Pomp Night Stoping</caption>\n";

h += "     <tr>\n";
h += "         <td>Pomp Night Enable\n";
h += "         <td><input type='text' name='PompNightEnable' value='" + String(PompNightEnable) + "' form='set'>\n";
h += "     </tr>\n";

h += "     <tr>\n";
h += "         <td>Pomp Night Pomp\n";
h += "        <td><input type='text' name='PompNightPomp' value='" + PompNightPomp + "' form='set'>\n";
h += "     </tr>\n";


h += "     <tr>\n";
h += "         <td>MinLightLevel\n";
h += "        <td><input type='text' name='MinLightLevel' value='" + String(MinLightLevel) + "' form='set'>\n";
h += "     </tr>\n";

h += "</table>\n";
h += "<input type='submit' value='save' form='set'/></form>\n";

#endif // c_MCP23017