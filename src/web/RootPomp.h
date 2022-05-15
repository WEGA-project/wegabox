// h += "<form action='' method='POST' id='set'></form>\n";
// h += "<table><caption>Root pomp</caption>\n";

if (server.arg("RootPomp") != "")
  preferences.putInt("RootPomp", server.arg("RootPomp").toInt());

if (server.arg("SelectedRootPomp") != "")
  preferences.putString("SelectedRootPomp", server.arg("SelectedRootPomp"));
  
// h += "<tr><td>RootPomp<td><input type='text' name='RootPomp' value='" + fFTS(preferences.getInt("RootPomp", 0), 0) + "' form='set'></tr>\n";
// h += "</table>\n";
// h += "<input type='submit' value='save' form='set'/></form>\n";

String RootPomp=fFTS(preferences.getInt("RootPomp", 0), 0);
String SelectedRootPomp=preferences.getString("SelectedRootPomp", "DRV1_A");

h +="<form action='' method='POST' id='set'></form>\n";
h +=" <table>\n";
h +="     <caption>Root pomp</caption>\n";

h +="     <tr>\n";
h +="         <td>Selected Root Pomp\n";
h +="        <td><input type='text' name='SelectedRootPomp' value='"+ SelectedRootPomp +"' form='set'>\n";
h +="     </tr>\n";

h +="     <tr>\n";
h +="         <td>Autocontrol Root Pomp Enable\n";
h +="         <td><input type='text' name='RootPomp' value='" + RootPomp + "' form='set'>\n";
h +="     </tr>\n";

h +=" </table>\n";
h +=" <input type='submit' value='save' form='set' /></form>\n";
h +=" <form action='' method='POST' id='set'></form>\n";