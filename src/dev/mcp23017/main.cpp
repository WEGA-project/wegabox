#if c_MCP23017 == 1
#include <Adafruit_MCP23X17.h>
Adafruit_MCP23X17 mcp;

int pwd_val, pwd_freq, pwd_port;
int pwd_val2, pwd_freq2, pwd_port2;
int RootPwdMin, RootPwdMax, RootDistMin;

unsigned long MCP23017_old = millis();
unsigned long MCP23017_Repeat = 5000;
#include <dev/mcp23017/func.h>

#endif // c_MCP23017