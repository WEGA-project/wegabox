#if c_MCP23017 == 1


if (server.arg("ECDoserEnable") != "")
  preferences.putInt("ECDoserEnable", server.arg("ECDoserEnable").toInt());
  int ECDoserEnable=preferences.getInt("ECDoserEnable", 0);

if (server.arg("ECDoserLimit") != "")
  preferences.putFloat("ECDoserLimit", server.arg("ECDoserLimit").toFloat());
  float ECDoserLimit=preferences.getFloat("ECDoserLimit", 1.5);

if (server.arg("ECDoserValueA") != "")
  preferences.putFloat("ECDoserValueA", server.arg("ECDoserValueA").toFloat());
  float ECDoserValueA=preferences.getFloat("ECDoserValueA", 1);

if (server.arg("ECDoserValueB") != "")
  preferences.putFloat("ECDoserValueB", server.arg("ECDoserValueB").toFloat());
  float ECDoserValueB=preferences.getFloat("ECDoserValueB", 1);

if (server.arg("ECDoserInterval") != "")
  preferences.putInt("ECDoserInterval", server.arg("ECDoserInterval").toInt());
  int ECDoserInterval=preferences.getInt("ECDoserInterval", 600);

h +="<form action='' method='POST' id='set'></form>\n";
h +=" <table>\n";
h +="     <caption>EC Doser correction</caption>\n";

h +="     <tr>\n";
h +="         <td>EC Doser Stab Enable (0-off, 1-on)\n";
h +="         <td><input type='text' name='ECDoserEnable' value='"  + String(ECDoserEnable) + "' form='set'>\n";
h +="     </tr>\n";

h +="     <tr>\n";
h +="         <td>ECDoserLimit\n";
h +="        <td><input type='text' name='ECDoserLimit' value='"+ fFTS(ECDoserLimit,3) +"' form='set'>\n";
h +="     </tr>\n";

h +="     <tr>\n";
h +="         <td>EC Doser Value A (ml)\n";
h +="        <td><input type='text' name='ECDoserValueA' value='"+ fFTS(ECDoserValueA,3) +"' form='set'>\n";
h +="     </tr>\n";

h +="     <tr>\n";
h +="         <td>EC Doser Value B (ml)\n";
h +="        <td><input type='text' name='ECDoserValueB' value='"+ fFTS(ECDoserValueB,3) +"' form='set'>\n";
h +="     </tr>\n";

h +="     <tr>\n";
h +="         <td>EC Doser Interval (sec)\n";
h +="         <td><input type='text' name='ECDoserInterval' value='" + String(ECDoserInterval) + "' form='set'>\n";
h +="     </tr>\n";


h += "</table>\n";
h += "<input type='submit' value='save' form='set'/></form>\n";
#endif // c_MCP23017