void handleSettings()
{
       
    //preferences.putFloat("param1", 21.1);

  //float dataF_read=preferences.getFloat("param1", 0);



  // if(server.arg("PWD") != "")  preferences.putInt("PWD", server.arg("PWD").toInt());
  // if(server.arg("FREQ") != "")  preferences.putInt("FREQ", server.arg("FREQ").toInt());
    //float p1f=server.arg("p1").toFloat(), p1c=(p1-p1f)/p1f*100;

  syslog_ng("WEB /settings");

  String h = "<!DOCTYPE html> <html>\n";
  h += "HOSTNAME=" + String(HOSTNAME) + "<br>\n";
  h += "Firmware=" + String(Firmware) + "<br>\n";
  //h += fFTS(dataF_read,2) + "<br>\n";
  h +="<form action='' method='POST' id='set'></form>\n";
  h +="<table><caption>PWD Settings</caption>\n";

  if(server.arg("PWDport") != "")  preferences.putInt("PWDport", server.arg("PWDport").toInt());
  h +="<tr><td>PWDport<td><input type='text' name='PWDport' value='" + fFTS(preferences.getInt("PWDport", 0),0) + "' form='set'></tr>\n";

  if(server.arg("PWD") != "")  preferences.putInt("PWD", server.arg("PWD").toInt());
  h +="<tr><td>PWD<td><input type='text' name='PWD' value='" + fFTS(preferences.getInt("PWD", 0),0) + "' form='set'></tr>\n";

  if(server.arg("FREQ") != "")  preferences.putInt("FREQ", server.arg("FREQ").toInt());
  h +="<tr><td>FREQ<td><input type='text' name='FREQ' value='" + fFTS(preferences.getInt("FREQ", 0),0) + "' form='set'></tr>\n";

  h +="</table>\n";

  h +="<table><caption>MCP23017 Status ports</caption>\n";  
  if(server.arg("MCP23017_p0") != "")  preferences.putInt("MCP23017_p0", server.arg("MCP23017_p0").toInt());
  if(server.arg("MCP23017_p1") != "")  preferences.putInt("MCP23017_p1", server.arg("MCP23017_p1").toInt());
  if(server.arg("MCP23017_p2") != "")  preferences.putInt("MCP23017_p2", server.arg("MCP23017_p2").toInt());
  if(server.arg("MCP23017_p3") != "")  preferences.putInt("MCP23017_p3", server.arg("MCP23017_p3").toInt());

  h +="<tr><td>MCP23017_p0<td><input type='text' name='MCP23017_p0' value='" + fFTS(preferences.getInt("MCP23017_p0", 0),0) + "' form='set'></tr>\n";
  h +="<tr><td>MCP23017_p1<td><input type='text' name='MCP23017_p1' value='" + fFTS(preferences.getInt("MCP23017_p1", 0),0) + "' form='set'></tr>\n";
  h +="<tr><td>MCP23017_p2<td><input type='text' name='MCP23017_p2' value='" + fFTS(preferences.getInt("MCP23017_p2", 0),0) + "' form='set'></tr>\n";
  h +="<tr><td>MCP23017_p3<td><input type='text' name='MCP23017_p3' value='" + fFTS(preferences.getInt("MCP23017_p3", 0),0) + "' form='set'></tr>\n";

  h +="</table>\n";


  h +="<input type='submit' value='save' form='set'/></form>\n";
 

    server.send(200, "text/html", h);
}

// <form method="post" id="myform"></form>
// <table> 
//    <tr> 
//       <td> 
//          <input type="text" name="first_name" form="myform">
//       </td>
//       <td> 
//          <input type="text" name="second_name" form="myform"> 
//       </td>
//       <td> 
//          <input type="submit" name="add"  form="myform"/> 
//       </td> 
//    </tr> 
// </table>
 