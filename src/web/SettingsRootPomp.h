// h += "<form action='' method='POST' id='set'></form>\n";
// h += "<table><caption>Root pomp</caption>\n";
#if c_MCP23017 == 1

if (server.arg("RootPomp") != "")
  preferences.putInt("RootPomp", server.arg("RootPomp").toInt());

if (server.arg("SelectedRootPomp") != "")
  preferences.putString("SelectedRootPomp", server.arg("SelectedRootPomp"));

if (server.arg("RootPwdOn") != "")
{
  preferences.putInt("RootPwdOn", server.arg("RootPwdOn").toInt());
}
RootPwdOn = preferences.getInt("RootPwdOn", 1);


if (server.arg("RootPwdMax") != "")
{
  preferences.putInt("RootPwdMax", server.arg("RootPwdMax").toInt());
}
RootPwdMax = preferences.getInt("RootPwdMax", 254);

if (server.arg("RootPwdMin") != "")
{
  preferences.putInt("RootPwdMin", server.arg("RootPwdMin").toInt());
}
RootPwdMin = preferences.getInt("RootPwdMin", 0);

if (server.arg("RootDistMin") != "")
{
  preferences.putInt("RootDistMin", server.arg("RootDistMin").toInt());
}
RootDistMin = preferences.getInt("RootDistMin", 0);

if (server.arg("PwdStepUp") != "")
{
  preferences.putInt("PwdStepUp", server.arg("PwdStepUp").toInt());
}
PwdStepUp = preferences.getInt("PwdStepUp", 1);

if (server.arg("PwdStepDown") != "")
{
  preferences.putInt("PwdStepDown", server.arg("PwdStepDown").toInt());
}
PwdStepDown = preferences.getInt("PwdStepDown", 10);

String RootPomp = fFTS(preferences.getInt("RootPomp", 0), 0);
String SelectedRootPomp = preferences.getString("SelectedRootPomp", "DRV1_A");
// h +="<details>\n";
// h +="<summary>Root pressure management</summary>\n";
h += "<form action='' method='POST' id='set'></form>\n";
h += " <table>\n";
h += "     <caption>Root pressure management</caption>\n";

h += "     <tr>\n";
h += "         <td>Selected Root Pomp\n";
h += "        <td><input type='text' name='SelectedRootPomp' value='" + SelectedRootPomp + "' form='set'>\n";
h += "     </tr>\n";

h += "     <tr>\n";
h += "         <td>Autocontrol Enable\n";
h += "         <td><input type='text' name='RootPomp' value='" + RootPomp + "' form='set'>\n";
h += "     </tr>\n";

// Использовать ли шим 0 - нет 1 - да
h += "     <tr>\n";
h += "         <td>Root Pwd On\n";
h += "         <td><input type='text' name='RootPwdOn' value='" + String(RootPwdOn) + "' form='set'>\n";
h += "     </tr>\n";

// Максимальный уровень шим
h += "     <tr>\n";
h += "         <td>Pwd Maximum\n";
h += "         <td><input type='text' name='RootPwdMax' value='" + String(RootPwdMax) + "' form='set'>\n";
h += "     </tr>\n";

// Минимальный уровень шим
h += "     <tr>\n";
h += "         <td>Root Pwd Minimum\n";
h += "         <td><input type='text' name='RootPwdMin' value='" + String(RootPwdMin) + "' form='set'>\n";
h += "     </tr>\n";

// Минимальная дистанция до раствора в баке
h += "     <tr>\n";
h += "         <td>Dist Minimum\n";
h += "         <td><input type='text' name='RootDistMin' value='" + String(RootDistMin) + "' form='set'>\n";
h += "     </tr>\n";

// Шаг шим при увеличении
h += "     <tr>\n";
h += "         <td>Pwd Step Up\n";
h += "         <td><input type='text' name='PwdStepUp' value='" + String(PwdStepUp) + "' form='set'>\n";
h += "     </tr>\n";

// Шаг шим при уменьшении
h += "     <tr>\n";
h += "         <td>Pwd Step Down\n";
h += "         <td><input type='text' name='PwdStepDown' value='" + String(PwdStepDown) + "' form='set'>\n";
h += "     </tr>\n";

h += " </table>\n";
h += " <input type='submit' value='save' form='set' /></form>\n";
h += " <form action='' method='POST' id='set'></form>\n";
// h +="</details>\n";

#endif // c_MCP23017