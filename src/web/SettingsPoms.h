
#if c_MCP23017 == 1

if (preferences.getInt("PWDport", -1) == -1)
  preferences.putInt("PWDport", 16);

if (preferences.getInt("PWD", -1) == -1)
  preferences.putInt("PWD", 0);

if (preferences.getInt("FREQ", -1) == -1)
  preferences.putInt("FREQ", 5000);

if (preferences.getInt("PWDport2", -1) == -1)
  preferences.putInt("PWDport2", 17);

if (preferences.getInt("PWD2", -1) == -1)
  preferences.putInt("PWD2", 0);

if (preferences.getInt("FREQ2", -1) == -1)
  preferences.putInt("FREQ2", 5000);

h += "<form action='' method='POST' id='set'></form>\n";

/////////PWD2 GROUP//////////////
h += "<table><caption>PWD group 1</caption>\n";

if (server.arg("PWDport") != "")
  preferences.putInt("PWDport", server.arg("PWDport").toInt());
h += "<tr><td>PWDport<td><input type='text' name='PWDport' value='" + fFTS(preferences.getInt("PWDport", 0), 0) + "' form='set'></tr>\n";

if (server.arg("PWD") != "")
  preferences.putInt("PWD", server.arg("PWD").toInt());
h += "<tr><td>PWD<td><input type='text' name='PWD' value='" + fFTS(preferences.getInt("PWD", 0), 0) + "' form='set'></tr>\n";

if (server.arg("FREQ") != "")
  preferences.putInt("FREQ", server.arg("FREQ").toInt());
h += "<tr><td>FREQ<td><input type='text' name='FREQ' value='" + fFTS(preferences.getInt("FREQ", 0), 0) + "' form='set'></tr>\n";

// DRV1 setting state PWD1 group

if (server.arg("DRV1_A_State") != "")
  preferences.putInt("DRV1_A_State", server.arg("DRV1_A_State").toInt());
h += "<tr><td>DRV1_A_State<td><input type='text' name='DRV1_A_State' value='" + fFTS(preferences.getInt("DRV1_A_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 0))+"</tr>\n";


if (server.arg("DRV1_B_State") != "")
  preferences.putInt("DRV1_B_State", server.arg("DRV1_B_State").toInt());
h += "<tr><td>DRV1_B_State<td><input type='text' name='DRV1_B_State' value='" + fFTS(preferences.getInt("DRV1_B_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 1))+"</tr>\n";

// DRV2 setting state PWD1 group

if (server.arg("DRV2_A_State") != "")
  preferences.putInt("DRV2_A_State", server.arg("DRV2_A_State").toInt());
h += "<tr><td>DRV2_A_State<td><input type='text' name='DRV2_A_State' value='" + fFTS(preferences.getInt("DRV2_A_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 4))+"</tr>\n";


if (server.arg("DRV2_B_State") != "")
  preferences.putInt("DRV2_B_State", server.arg("DRV2_B_State").toInt());
h += "<tr><td>DRV2_B_State<td><input type='text' name='DRV2_B_State' value='" + fFTS(preferences.getInt("DRV2_B_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 5))+"</tr>\n";

// DRV3 setting state PWD1 group

if (server.arg("DRV3_A_State") != "")
  preferences.putInt("DRV3_A_State", server.arg("DRV3_A_State").toInt());
h += "<tr><td>DRV3_A_State<td><input type='text' name='DRV3_A_State' value='" + fFTS(preferences.getInt("DRV3_A_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 8))+"</tr>\n";

if (server.arg("DRV3_B_State") != "")
  preferences.putInt("DRV3_B_State", server.arg("DRV3_B_State").toInt());
h += "<tr><td>DRV3_B_State<td><input type='text' name='DRV3_B_State' value='" + fFTS(preferences.getInt("DRV3_B_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 9))+"</tr>\n";

// DRV4 setting state PWD1 group

if (server.arg("DRV4_A_State") != "")
  preferences.putInt("DRV4_A_State", server.arg("DRV4_A_State").toInt());
h += "<tr><td>DRV4_A_State<td><input type='text' name='DRV4_A_State' value='" + fFTS(preferences.getInt("DRV4_A_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 12))+"</tr>\n";

if (server.arg("DRV4_B_State") != "")
  preferences.putInt("DRV4_B_State", server.arg("DRV4_B_State").toInt());
h += "<tr><td>DRV4_B_State<td><input type='text' name='DRV4_B_State' value='" + fFTS(preferences.getInt("DRV4_B_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 13))+"</tr>\n";
h += "</table>\n";
///////////PWD2 GROUP//////////////
h += "<table><caption>PWD group 2</caption>\n";

if (server.arg("PWDport2") != "")
  preferences.putInt("PWDport2", server.arg("PWDport2").toInt());
h += "<tr><td>PWDport2<td><input type='text' name='PWDport2' value='" + fFTS(preferences.getInt("PWDport2", 0), 0) + "' form='set'></tr>\n";

if (server.arg("PWD2") != "")
  preferences.putInt("PWD2", server.arg("PWD2").toInt());
h += "<tr><td>PWD2<td><input type='text' name='PWD2' value='" + fFTS(preferences.getInt("PWD2", 0), 0) + "' form='set'></tr>\n";

if (server.arg("FREQ2") != "")
  preferences.putInt("FREQ2", server.arg("FREQ2").toInt());
h += "<tr><td>FREQ2<td><input type='text' name='FREQ2' value='" + fFTS(preferences.getInt("FREQ2", 0), 0) + "' form='set'></tr>\n";

// DRV1 setting state PWD2 group

if (server.arg("DRV1_C_State") != "")
  preferences.putInt("DRV1_C_State", server.arg("DRV1_C_State").toInt());
h += "<tr><td>DRV1_C_State<td><input type='text' name='DRV1_C_State' value='" + fFTS(preferences.getInt("DRV1_C_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 2))+"</tr>\n";

if (server.arg("DRV1_D_State") != "")
  preferences.putInt("DRV1_D_State", server.arg("DRV1_D_State").toInt());
h += "<tr><td>DRV1_D_State<td><input type='text' name='DRV1_D_State' value='" + fFTS(preferences.getInt("DRV1_D_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 3))+"</tr>\n";

// DRV2 setting state PWD2 group

if (server.arg("DRV2_C_State") != "")
  preferences.putInt("DRV2_C_State", server.arg("DRV2_C_State").toInt());
h += "<tr><td>DRV2_C_State<td><input type='text' name='DRV2_C_State' value='" + fFTS(preferences.getInt("DRV2_C_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 6))+"</tr>\n";

if (server.arg("DRV2_D_State") != "")
  preferences.putInt("DRV2_D_State", server.arg("DRV2_D_State").toInt());
h += "<tr><td>DRV2_D_State<td><input type='text' name='DRV2_D_State' value='" + fFTS(preferences.getInt("DRV2_D_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 7))+"</tr>\n";

// DRV3 setting state PWD2 group

if (server.arg("DRV3_C_State") != "")
  preferences.putInt("DRV3_C_State", server.arg("DRV3_C_State").toInt());
h += "<tr><td>DRV3_C_State<td><input type='text' name='DRV3_C_State' value='" + fFTS(preferences.getInt("DRV3_C_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 10))+"</tr>\n";

if (server.arg("DRV3_D_State") != "")
  preferences.putInt("DRV3_D_State", server.arg("DRV3_D_State").toInt());
h += "<tr><td>DRV3_D_State<td><input type='text' name='DRV3_D_State' value='" + fFTS(preferences.getInt("DRV3_D_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 11))+"</tr>\n";

// DRV4 setting state PWD2 group

if (server.arg("DRV4_C_State") != "")
  preferences.putInt("DRV4_C_State", server.arg("DRV4_C_State").toInt());
h += "<tr><td>DRV4_C_State<td><input type='text' name='DRV4_C_State' value='" + fFTS(preferences.getInt("DRV4_C_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 14))+"</tr>\n";

if (server.arg("DRV4_D_State") != "")
  preferences.putInt("DRV4_D_State", server.arg("DRV4_D_State").toInt());
h += "<tr><td>DRV4_D_State<td><input type='text' name='DRV4_D_State' value='" + fFTS(preferences.getInt("DRV4_D_State", 0), 0) + "' form='set'><td>"+String(bitRead(readGPIO, 15))+"</tr>\n";

h += "</table>\n";
h += "<input type='submit' value='save' form='set'/></form>\n";


#endif // c_MCP23017
