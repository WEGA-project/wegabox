#if c_MCP23017 == 1

if (server.arg("ECStabEnable") != "")
  preferences.putInt("ECStabEnable", server.arg("ECStabEnable").toInt());
  int ECStabEnable=preferences.getInt("ECStabEnable", 0);

if (server.arg("ECStabPomp") != "")
  preferences.putString("ECStabPomp", server.arg("ECStabPomp"));
  String ECStabPomp=preferences.getString("ECStabPomp", "DRV1_D");

if (server.arg("ECStabValue") != "")
  preferences.putFloat("ECStabValue", server.arg("ECStabValue").toFloat());
  float ECStabValue=preferences.getFloat("ECStabValue", 2.5);

if (server.arg("ECStabTime") != "")
  preferences.putInt("ECStabTime", server.arg("ECStabTime").toInt());
  int ECStabTime=preferences.getInt("ECStabTime", 20);

if (server.arg("ECStabInterval") != "")
  preferences.putInt("ECStabInterval", server.arg("ECStabInterval").toInt());
  int ECStabInterval=preferences.getInt("ECStabInterval", 180);

if (server.arg("ECStabMinDist") != "")
  preferences.putFloat("ECStabMinDist", server.arg("ECStabMinDist").toFloat());
  float ECStabMinDist=preferences.getFloat("ECStabMinDist", 5);

if (server.arg("ECStabMaxDist") != "")
  preferences.putFloat("ECStabMaxDist", server.arg("ECStabMaxDist").toFloat());
  float ECStabMaxDist=preferences.getFloat("ECStabMaxDist", 50);

h +="<form action='' method='POST' id='set'></form>\n";
h +=" <table>\n";
h +="     <caption>EC correction</caption>\n";


h +="     <tr>\n";
h +="         <td>EC Stab Enable\n";
h +="         <td><input type='text' name='ECStabEnable' value='" + String(ECStabEnable) + "' form='set'>\n";
h +="     </tr>\n";


h +="     <tr>\n";
h +="         <td>EC Stab Pomp\n";
h +="        <td><input type='text' name='ECStabPomp' value='"+ ECStabPomp +"' form='set'>\n";
h +="     </tr>\n";

h +="     <tr>\n";
h +="         <td>EC Stab Value\n";
h +="        <td><input type='text' name='ECStabValue' value='"+ fFTS(ECStabValue,3) +"' form='set'>\n";
h +="     </tr>\n";

h +="     <tr>\n";
h +="         <td>EC Stab Time\n";
h +="         <td><input type='text' name='ECStabTime' value='" + String(ECStabTime) + "' form='set'>\n";
h +="     </tr>\n";

h +="     <tr>\n";
h +="         <td>EC Stab Interval\n";
h +="         <td><input type='text' name='ECStabInterval' value='" + String(ECStabInterval) + "' form='set'>\n";
h +="     </tr>\n";

h +="     <tr>\n";
h +="         <td>EC Stab Miniman Dist\n";
h +="        <td><input type='text' name='ECStabMinDist' value='"+ fFTS(ECStabMinDist,1) +"' form='set'>\n";
h +="     </tr>\n";

h +="     <tr>\n";
h +="         <td>EC Stab Maximal Dist\n";
h +="        <td><input type='text' name='ECStabMaxDist' value='"+ fFTS(ECStabMaxDist,1) +"' form='set'>\n";
h +="     </tr>\n";

h += "</table>\n";
h += "<input type='submit' value='save' form='set'/></form>\n";
#endif // c_MCP23017