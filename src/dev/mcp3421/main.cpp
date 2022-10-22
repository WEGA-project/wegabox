#if c_MCP3421 == 1
#include <MCP342x.h>
uint8_t MCP3421addr = 0x68;
MCP342x adc2 = MCP342x(MCP3421addr);
unsigned long MCP3421_old = millis();
unsigned long MCP3421_Repeat = 20000;
#endif