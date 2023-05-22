#if c_MCP23017 == 1 and c_DOSER == 1
if (server.arg("SetPumpA_Ml") != "")  preferences.putFloat ("SetPumpA_Ml", server.arg("SetPumpA_Ml").toFloat());
if (server.arg("SetPumpB_Ml") != "")  preferences.putFloat ("SetPumpB_Ml", server.arg("SetPumpB_Ml").toFloat());
SetPumpA_Ml=preferences.getFloat("SetPumpA_Ml", 0);
SetPumpB_Ml=preferences.getFloat("SetPumpB_Ml", 0);


if (server.arg("StPumpA_Del") != "")  preferences.putInt("StPumpA_Del", server.arg("StPumpA_Del").toInt());
if (server.arg("StPumpA_Ret") != "")  preferences.putInt("StPumpA_Ret", server.arg("StPumpA_Ret").toInt());

if (server.arg("StPumpA_On") != "")  preferences.putInt("StPumpA_On", server.arg("StPumpA_On").toInt());

if (server.arg("StPumpA_cStepMl") != "")  preferences.putFloat ("StPumpA_cStepMl", server.arg("StPumpA_cStepMl").toFloat());
if (server.arg("StPumpA_cMl") != "")  preferences.putFloat ("StPumpA_cMl", server.arg("StPumpA_cMl").toFloat());
if (server.arg("StPumpA_cStep") != "")  preferences.putFloat ("StPumpA_cStep", server.arg("StPumpA_cStep").toFloat());



if (server.arg("StPumpA_A") != "")  preferences.putInt("StPumpA_A", server.arg("StPumpA_A").toInt());
if (server.arg("StPumpA_B") != "")  preferences.putInt("StPumpA_B", server.arg("StPumpA_B").toInt());
if (server.arg("StPumpA_C") != "")  preferences.putInt("StPumpA_C", server.arg("StPumpA_C").toInt());
if (server.arg("StPumpA_D") != "")  preferences.putInt("StPumpA_D", server.arg("StPumpA_D").toInt());

if (server.arg("StPumpB_Del") != "")  preferences.putInt("StPumpB_Del", server.arg("StPumpB_Del").toInt());
if (server.arg("StPumpB_Ret") != "")  preferences.putInt("StPumpB_Ret", server.arg("StPumpB_Ret").toInt());

if (server.arg("StPumpB_On") != "")  preferences.putInt("StPumpB_On", server.arg("StPumpB_On").toInt());

if (server.arg("StPumpB_cStepMl") != "")  preferences.putFloat ("StPumpB_cStepMl", server.arg("StPumpB_cStepMl").toFloat());
if (server.arg("StPumpB_cMl") != "")  preferences.putFloat ("StPumpB_cMl", server.arg("StPumpB_cMl").toFloat());
if (server.arg("StPumpB_cStep") != "")  preferences.putFloat ("StPumpB_cStep", server.arg("StPumpB_cStep").toFloat());


if (server.arg("StPumpB_A") != "")  preferences.putInt("StPumpB_A", server.arg("StPumpB_A").toInt());
if (server.arg("StPumpB_B") != "")  preferences.putInt("StPumpB_B", server.arg("StPumpB_B").toInt());
if (server.arg("StPumpB_C") != "")  preferences.putInt("StPumpB_C", server.arg("StPumpB_C").toInt());
if (server.arg("StPumpB_D") != "")  preferences.putInt("StPumpB_D", server.arg("StPumpB_D").toInt());

ADel=preferences.getInt("StPumpA_Del", 700);
ARet=preferences.getInt("StPumpA_Ret", 700);
AOn=preferences.getInt("StPumpA_On", 0);
StPumpA_cStepMl=preferences.getFloat("StPumpA_cStepMl", 1000);
StPumpA_cMl=preferences.getFloat("StPumpA_cMl", 1);
StPumpA_cStep=preferences.getFloat("StPumpA_cStep", 1000);


AA=preferences.getInt("StPumpA_A", 4);
AB=preferences.getInt("StPumpA_B", 5);
AC=preferences.getInt("StPumpA_C", 6);
AD=preferences.getInt("StPumpA_D", 7);

BDel=preferences.getInt("StPumpB_Del", 700);
BRet=preferences.getInt("StPumpB_Ret", 700);
BOn=preferences.getInt("StPumpB_On", 0);
StPumpB_cStepMl=preferences.getFloat("StPumpB_cStepMl", 1000);
StPumpB_cMl=preferences.getFloat("StPumpB_cMl", 1);
StPumpB_cStep=preferences.getFloat("StPumpB_cStep", 1000);

BA=preferences.getInt("StPumpB_A", 8);
BB=preferences.getInt("StPumpB_B", 9);
BC=preferences.getInt("StPumpB_C", 10);
BD=preferences.getInt("StPumpB_D", 11);


h += "<form action='' method='POST' id='set'></form>\n";

/////////   A  //////////////
h += "<table><caption>Doser Steping Pump A</caption>\n";

h += "<tr><td>SetPumpA_Ml<td><input type='text' name='SetPumpA_Ml' value='" + fFTS(SetPumpA_Ml,3) + "' form='set'></tr>\n";

h += "<tr><td>StPumpA_cStepMl<td><input type='text' name='StPumpA_cStepMl' value='" + fFTS(StPumpA_cStepMl,3) + "' form='set'></tr>\n";
h += "<tr><td>StPumpA_cMl<td><input type='text' name='StPumpA_cMl' value='" + fFTS(StPumpA_cMl,3) + "' form='set'></tr>\n";
h += "<tr><td>StPumpA_cStep<td><input type='text' name='StPumpA_cStep' value='" + fFTS(StPumpA_cStep,3) + "' form='set'></tr>\n";

h += "<tr><td>StPumpA_Del<td><input type='text' name='StPumpA_Del' value='" + String(ADel) + "' form='set'></tr>\n";
h += "<tr><td>StPumpA_Ret<td><input type='text' name='StPumpA_Ret' value='" + String(ARet) + "' form='set'></tr>\n";

h += "<tr><td>StPumpA_On<td><input type='text' name='StPumpA_On' value='" + String(AOn) + "' form='set'></tr>\n";

h += "<tr><td>StPumpA_A<td><input type='text' name='StPumpA_A' value='" + String(AA) + "' form='set'></tr>\n";
h += "<tr><td>StPumpA_B<td><input type='text' name='StPumpA_B' value='" + String(AB) + "' form='set'></tr>\n";
h += "<tr><td>StPumpA_C<td><input type='text' name='StPumpA_C' value='" + String(AC) + "' form='set'></tr>\n";
h += "<tr><td>StPumpA_D<td><input type='text' name='StPumpA_D' value='" + String(AD) + "' form='set'></tr>\n";

h += "</table>\n";


/////////// B  //////////////
h += "<table><caption>Doser Steping Pump B</caption>\n";

h += "<tr><td>SetPumpB_Ml<td><input type='text' name='SetPumpB_Ml' value='" + fFTS(SetPumpB_Ml,3) + "' form='set'></tr>\n";


h += "<tr><td>StPumpB_cStepMl<td><input type='text' name='StPumpB_cStepMl' value='" + fFTS(StPumpB_cStepMl,3) + "' form='set'></tr>\n";
h += "<tr><td>StPumpB_cMl<td><input type='text' name='StPumpB_cMl' value='" + fFTS(StPumpB_cMl,3) + "' form='set'></tr>\n";
h += "<tr><td>StPumpB_cStep<td><input type='text' name='StPumpB_cStep' value='" + fFTS(StPumpB_cStep,3) + "' form='set'></tr>\n";


h += "<tr><td>StPumpB_Del<td><input type='text' name='StPumpB_Del' value='" + String(BDel) + "' form='set'></tr>\n";
h += "<tr><td>StPumpB_Ret<td><input type='text' name='StPumpB_Ret' value='" + String(BRet) + "' form='set'></tr>\n";

h += "<tr><td>StPumpB_On<td><input type='text' name='StPumpB_On' value='" + String(BOn) + "' form='set'></tr>\n";

h += "<tr><td>StPumpB_A<td><input type='text' name='StPumpB_A' value='" + String(BA) + "' form='set'></tr>\n";
h += "<tr><td>StPumpB_B<td><input type='text' name='StPumpB_B' value='" + String(BB) + "' form='set'></tr>\n";
h += "<tr><td>StPumpB_C<td><input type='text' name='StPumpB_C' value='" + String(BC) + "' form='set'></tr>\n";
h += "<tr><td>StPumpB_D<td><input type='text' name='StPumpB_D' value='" + String(BD) + "' form='set'></tr>\n";

h += "</table>\n";

h += "<input type='submit' value='save' form='set'/></form>\n";


#endif // c_DOSER