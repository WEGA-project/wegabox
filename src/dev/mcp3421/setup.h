#if c_MCP3421 == 1
  MCP342x::generalCallReset();
  delay(1); // MC342x needs 300us to settle
    // Check device present
  Wire.requestFrom(address, (uint8_t)1);
  if (!Wire.available()) {
    syslog_err("MCP3421: No device found");
    //Serial.print("No device found at address ");
    //Serial.println(address, HEX);     
  }
  else 
  syslog_ng("MCP3421: Device found!");


xTaskCreate(TaskMCP3421,"TaskMCP3421",5000,NULL,1,NULL);
#endif // c_MCP3421