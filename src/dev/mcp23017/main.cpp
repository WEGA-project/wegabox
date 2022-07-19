#if c_MCP23017 == 1
#include <Adafruit_MCP23X17.h>
Adafruit_MCP23X17 mcp;

int pwd_val, pwd_freq, pwd_port;
int pwd_val2, pwd_freq2, pwd_port2;
int RootPwdMin, RootPwdMax, RootDistMin;
unsigned int bitw= 0b0000000000000000;
int pwd_val_root, RootPwdOn, PwdStepUp, PwdStepDown;
unsigned long MCP23017_old = millis();
unsigned long MCP23017_Repeat = 10000;
unsigned long ECStabTimeStart;
float aPWD=0;
#include <dev/mcp23017/func.h>

#endif // c_MCP23017