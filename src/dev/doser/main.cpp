#if c_MCP23017 == 1 and c_DOSER == 1
  long lastOffDoser;

    int AOn = 1;
    int ADel=700;
    int ARet=700;

    int AA = 4;
    int AB = 5;
    int AC = 6;
    int AD = 7;

    int BOn = 1;
    int BDel=700;
    int BRet=700;

    int BA = 8;
    int BB = 9;
    int BC = 10;
    int BD = 11;
    
    long del = 700;
    long ret = 1500;
    int stap = 1;

float StPumpA_cStepMl=1000;
float StPumpA_cMl=1;
float StPumpB_cStepMl=1000;
float StPumpB_cMl=1;

float SetPumpA_Ml=0;
float SetPumpB_Ml=0;

#include <dev/doser/func.h> 

unsigned long DOSER_old = millis();
unsigned long DOSER_Repeat = 20000;
#endif // c_DOSER