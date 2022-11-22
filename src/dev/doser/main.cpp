#if c_MCP23017 == 1 and c_DOSER == 1
  long lastOffDoser;

    int AA = 4;
    int AB = 5;
    int AC = 6;
    int AD = 7;

    int BA = 8;
    int BB = 9;
    int BC = 10;
    int BD = 11;
    
    long del = 700;
    int stap = 1;

unsigned long DOSER_old = millis();
unsigned long DOSER_Repeat = 20000;
#endif // c_DOSER