#if c_hall == 1


#define hall_MiddleCount 5000
unsigned long hall_old = millis();
unsigned long hall_Repeat = 120000;
RunningMedian HallRM = RunningMedian(15);
#endif //c_hall