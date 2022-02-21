#if c_MCP23017 == 1
  #include <Adafruit_MCP23X17.h>
  Adafruit_MCP23X17 mcp;
  #define PWD1 34
  #define PWD2 25
  #define DRV1_A 0
  #define DRV1_B 1
  #define DRV1_C 2
  #define DRV1_D 3
    unsigned long MCP23017_old = millis();
  unsigned long MCP23017_Repeat = 20000;

#endif // c_MCP23017