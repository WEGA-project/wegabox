#if c_MCP3421 == 1
  #include <MCP342x.h>
  uint8_t address = 0x68;
  MCP342x adc = MCP342x(address);
  unsigned long MCP3421_old = millis();
  unsigned long MCP3421_Repeat = 20000;
#endif