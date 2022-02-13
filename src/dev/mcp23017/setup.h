#if c_MCP23017 == 1
  if (!mcp.begin_I2C())
    syslog_ng("mcp23017 Error.");
  else
    syslog_ng("mcp23017 Start.");

// // задаём свойства ШИМ-сигнала
// const int freq = 300000;
// const int ledChannel = 0;
// const int resolution = 8;
// pwd=255;
// ledcSetup(ledChannel, freq, resolution);
// ledcAttachPin(PWD2, ledChannel);

//   ledcWrite(0, pwd);
//   mcp.pinMode(DRV1_A, OUTPUT);
//   mcp.digitalWrite(DRV1_A, HIGH);

xTaskCreate(TaskMCP23017,"MCP23017",10000,NULL,0,NULL);
#endif // c_MCP23017