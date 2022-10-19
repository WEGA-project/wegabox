#if c_MCP23017 == 1
while (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdFALSE);

if (!mcp.begin_I2C())
  syslog_ng("mcp23017 Begin Error.");
else
  syslog_ng("mcp23017 Begin.");

for (int p = 0; p < 16; p++){
  mcp.pinMode(p, OUTPUT);
  mcp.digitalWrite(p,0);
}

    xSemaphoreGive(xSemaphoreX);    


if (preferences.getInt("DRV1_A", -1) == -1)
  preferences.putInt("DRV1_A", 0);
if (preferences.getInt("DRV1_B", -1) == -1)
  preferences.putInt("DRV1_B", 1);
if (preferences.getInt("DRV1_C", -1) == -1)
  preferences.putInt("DRV1_C", 2);
if (preferences.getInt("DRV1_D", -1) == -1)
  preferences.putInt("DRV1_D", 3);
if (preferences.getInt("DRV2_A", -1) == -1)
  preferences.putInt("DRV2_A", 4);
if (preferences.getInt("DRV2_B", -1) == -1)
  preferences.putInt("DRV2_B", 5);
if (preferences.getInt("DRV2_C", -1) == -1)
  preferences.putInt("DRV2_C", 6);
if (preferences.getInt("DRV2_D", -1) == -1)
  preferences.putInt("DRV2_D", 7);
if (preferences.getInt("DRV3_A", -1) == -1)
  preferences.putInt("DRV3_A", 8);
if (preferences.getInt("DRV3_B", -1) == -1)
  preferences.putInt("DRV3_B", 9);
if (preferences.getInt("DRV3_C", -1) == -1)
  preferences.putInt("DRV3_C", 10);
if (preferences.getInt("DRV3_D", -1) == -1)
  preferences.putInt("DRV3_D", 11);
if (preferences.getInt("DRV4_A", -1) == -1)
  preferences.putInt("DRV4_A", 12);
if (preferences.getInt("DRV4_B", -1) == -1)
  preferences.putInt("DRV4_B", 13);
if (preferences.getInt("DRV4_C", -1) == -1)
  preferences.putInt("DRV4_C", 14);
if (preferences.getInt("DRV4_D", -1) == -1)
  preferences.putInt("DRV4_D", 15);



xTaskCreate(TaskMCP23017, "MCP23017", 12000, NULL, 1, NULL);
#endif // c_MCP23017