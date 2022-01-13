#if c_MCP23017 == 1
  #include <Adafruit_MCP23X17.h>
  Adafruit_MCP23X17 mcp;
  #define PWD1 16
  #define PWD2 17
  #define DRV1_A 0
  #define DRV1_B 1
  #define DRV1_C 2
  #define DRV1_D 3

#endif // c_MCP23017