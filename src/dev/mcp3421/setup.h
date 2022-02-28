#if c_MCP3421 == 1
  MCP342x::generalCallReset();
  delay(1); // MC342x needs 300us to settle
    // Check device present
  Wire.requestFrom(address, (uint8_t)1);
  if (!Wire.available()) {
    Serial.print("No device found at address ");
    Serial.println(address, HEX);     
  }

xTaskCreate(TaskMCP3421,"TaskMCP3421",5000,NULL,0,NULL);
#endif // c_MCP3421