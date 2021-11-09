void handleStatus(){

  String statusstr="status ok \n";

  statusstr += "Uptime=" + fFTS(millis()/1000,0) + "\n";
  statusstr += "EC Times=" + fFTS(float(t_EC/1000),3)+ " msec\n";
  statusstr += "EC Freq=" + fFTS(f_EC,3)+ " Hz\n";

// DeviceAddress insideThermometer;
//   sens18b20.begin();
//   sens18b20.getDeviceCount();
//   if (sens18b20.readPowerSupply()) statusstr += "parasite power requirements=ON\n";  // no address means "scan all devices for parasite mode"
//   else statusstr += "parasite power requirements=OFF\n";
//   sens18b20.getAddress(insideThermometer, 0);
//  float ds18b20s = sens18b20.readPowerSupply(insideThermometer);
// statusstr += "ds18b20s=" + fFTS(ds18b20s,3)+ " C\n";



  // sens18b20.readPowerSupply();
//  sens18b20.begin();
      sens18b20.requestTemperatures();
      float ds18b20s = sens18b20.getTempCByIndex(0);
  statusstr += "ds18b20s=" + fFTS(ds18b20s,3)+ " C\n";  


  // // sens18b20.begin();
  // // sens18b20.readPowerSupply();
  // // Grab a count of devices on the wire
  // int numberOfDevices = sens18b20.getDeviceCount();
  //  statusstr += "ds18b20num=" + fFTS(numberOfDevices,0)+ " C\n";  
server.send(200, "text/plain",  statusstr);

}