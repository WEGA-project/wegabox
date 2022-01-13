#if c_MCP23017 == 1
  if (!mcp.begin_I2C()) {
    Serial.println("mcp23017 Error.");
    //while (1);
  }

// задаём свойства ШИМ-сигнала
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
ledcSetup(ledChannel, 150000, resolution);
ledcAttachPin(PWD1, ledChannel);

xTaskCreate(TaskMCP23017,"MCP23017",10000,NULL,0,&appTasks[appTaskCount++]);
#endif // c_MCP23017