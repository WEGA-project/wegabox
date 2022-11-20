#if c_MCP23017 == 1 and c_DOSER == 1
  long lastOffDoser;

    int A = 4;
    int B = 5;
    int C = 6;
    int D = 7;

    int BA = 8;
    int BB = 9;
    int BC = 10;
    int BD = 11;
    
    long del = 700;
    int stap = 1;

unsigned long DOSER_old = millis();
unsigned long DOSER_Repeat = 20000;
#endif // c_DOSER