void handleStatus(){

  String statusstr="status ok \n";

  statusstr += "Uptime=" + fFTS(millis()/1000,0) + "\n";
  statusstr += "EC Times=" + fFTS(float(t_EC/1000),3)+ " msec\n";
  statusstr += "EC Freq=" + fFTS(f_EC,3)+ " Hz\n";

server.send(200, "text/plain",  statusstr);

}