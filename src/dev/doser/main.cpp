#if c_MCP23017 == 1 and c_DOSER == 1
  long lastOffDoser;

    int A = preferences.getInt("DRV2_A", -1);;
    int B = preferences.getInt("DRV2_B", -1);;
    int C = preferences.getInt("DRV2_C", -1);;
    int D = preferences.getInt("DRV2_D", -1);;
    long del = 300;
    int stap = 1;

unsigned long DOSER_old = millis();
unsigned long DOSER_Repeat = 20000;
#endif // c_DOSER