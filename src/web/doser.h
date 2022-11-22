#if c_MCP23017 == 1 and c_DOSER == 1

if (server.arg("StPumpA_del") != "")  preferences.putInt("StPumpA_del", server.arg("StPumpA_del").toInt());
if (server.arg("StPumpA_On") != "")  preferences.putInt("StPumpA_On", server.arg("StPumpA_On").toInt());

if (server.arg("StPumpA_A") != "")  preferences.putInt("StPumpA_A", server.arg("StPumpA_A").toInt());
if (server.arg("StPumpA_B") != "")  preferences.putInt("StPumpA_B", server.arg("StPumpA_B").toInt());
if (server.arg("StPumpA_C") != "")  preferences.putInt("StPumpA_C", server.arg("StPumpA_C").toInt());
if (server.arg("StPumpA_D") != "")  preferences.putInt("StPumpA_D", server.arg("StPumpA_D").toInt());

if (server.arg("StPumpB_del") != "")  preferences.putInt("StPumpB_del", server.arg("StPumpB_del").toInt());
if (server.arg("StPumpB_On") != "")  preferences.putInt("StPumpB_On", server.arg("StPumpB_On").toInt());

if (server.arg("StPumpB_A") != "")  preferences.putInt("StPumpB_A", server.arg("StPumpB_A").toInt());
if (server.arg("StPumpB_B") != "")  preferences.putInt("StPumpB_B", server.arg("StPumpB_B").toInt());
if (server.arg("StPumpB_C") != "")  preferences.putInt("StPumpB_C", server.arg("StPumpB_C").toInt());
if (server.arg("StPumpB_D") != "")  preferences.putInt("StPumpB_D", server.arg("StPumpB_D").toInt());

ADel=preferences.getInt("StPumpA_Del", 700);
AOn=preferences.getInt("StPumpA_On", 0);
AA=preferences.getInt("StPumpA_A", 4);
AB=preferences.getInt("StPumpA_B", 5);
AC=preferences.getInt("StPumpA_C", 6);
AD=preferences.getInt("StPumpA_D", 7);

BDel=preferences.getInt("StPumpB_Del", 700);
BOn=preferences.getInt("StPumpB_On", 0);
BA=preferences.getInt("StPumpB_A", 8);
BB=preferences.getInt("StPumpB_B", 9);
BC=preferences.getInt("StPumpB_C", 10);
BD=preferences.getInt("StPumpB_D", 11);


h += "<form action='' method='POST' id='set'></form>\n";

/////////   A  //////////////
h += "<table><caption>Doser Steping Pump A</caption>\n";

h += "<tr><td>StPumpA_Del<td><input type='text' name='StPumpA_Del' value='" + String(ADel) + "' form='set'></tr>\n";
h += "<tr><td>StPumpA_On<td><input type='text' name='StPumpA_On' value='" + String(AOn) + "' form='set'></tr>\n";

h += "<tr><td>StPumpA_A<td><input type='text' name='StPumpA_A' value='" + String(AA) + "' form='set'></tr>\n";
h += "<tr><td>StPumpA_B<td><input type='text' name='StPumpA_B' value='" + String(AB) + "' form='set'></tr>\n";
h += "<tr><td>StPumpA_C<td><input type='text' name='StPumpA_C' value='" + String(AC) + "' form='set'></tr>\n";
h += "<tr><td>StPumpA_D<td><input type='text' name='StPumpA_D' value='" + String(AD) + "' form='set'></tr>\n";

h += "</table>\n";


/////////// B  //////////////
h += "<table><caption>Doser Steping Pump B</caption>\n";

h += "<tr><td>StPumpB_Del<td><input type='text' name='StPumpB_Del' value='" + String(BDel) + "' form='set'></tr>\n";
h += "<tr><td>StPumpB_On<td><input type='text' name='StPumpB_On' value='" + String(BOn) + "' form='set'></tr>\n";

h += "<tr><td>StPumpB_A<td><input type='text' name='StPumpB_A' value='" + String(BA) + "' form='set'></tr>\n";
h += "<tr><td>StPumpB_B<td><input type='text' name='StPumpB_B' value='" + String(BB) + "' form='set'></tr>\n";
h += "<tr><td>StPumpB_C<td><input type='text' name='StPumpB_C' value='" + String(BC) + "' form='set'></tr>\n";
h += "<tr><td>StPumpB_D<td><input type='text' name='StPumpB_D' value='" + String(BD) + "' form='set'></tr>\n";

h += "</table>\n";

h += "<input type='submit' value='save' form='set'/></form>\n";









// h += "<form action='' method='POST' id='set'></form>\n";
// h += " <table>\n";
// h += "     <caption>Pomp Night Stoping</caption>\n";

// h += "     <tr>\n";
// h += "         <td>Pomp Night Enable\n";
// h += "         <td><input type='text' name='PompNightEnable' value='" + String(PompNightEnable) + "' form='set'>\n";
// h += "     </tr>\n";

// h += "</table>\n";
// h += "<input type='submit' value='save' form='set'/></form>\n";

#endif // c_DOSER