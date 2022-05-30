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


h +="<form action='' method='POST' id='set'></form>\n";
h +=" <table>\n";
h +="     <caption>EC correction</caption>\n";

h += "<tr><td>EC<td>" + fFTS(wEC,3) + "mS/cm</tr>\n";


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

h += "</table>\n";
h += "<input type='submit' value='save' form='set'/></form>\n";
#endif // c_MCP23017