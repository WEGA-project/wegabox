#if c_vcc == 1
extern "C" int rom_phy_get_vdd33();
float VccAdj = 1;

#define vcc_MiddleCount 5000
unsigned long vcc_old = millis();
unsigned long vcc_Repeat = 10000;
RunningMedian vccRM = RunningMedian(250);
#endif //c_vcc