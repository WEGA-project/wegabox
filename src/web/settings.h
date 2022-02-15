void handleSettings()
{
       
    //preferences.putFloat("param1", 21.1);

  //float dataF_read=preferences.getFloat("param1", 0);

  if(server.arg("PWD") != "")  preferences.putInt("PWD", server.arg("PWD").toInt());
  if(server.arg("FREQ") != "")  preferences.putInt("FREQ", server.arg("FREQ").toInt());


    //float p1f=server.arg("p1").toFloat(), p1c=(p1-p1f)/p1f*100;

  syslog_ng("WEB /settings");

  String h = "<!DOCTYPE html> <html>\n";
  h += "HOSTNAME=" + String(HOSTNAME) + "<br>\n";
  h += "Firmware=" + String(Firmware) + "<br>\n";
  //h += fFTS(dataF_read,2) + "<br>\n";
  h +="<form action='' method='GET' id='set'></form>\n";
  h +="<table>\n";
  h +="<tr><td>PWD<td><input type='text' name='PWD' value='" + fFTS(preferences.getInt("PWD", 0),0) + "' form='set'></tr>\n";
  h +="<tr><td>FREQ<td><input type='text' name='FREQ' value='" + fFTS(preferences.getInt("FREQ", 0),0) + "' form='set'></tr>\n";
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
 