#if c_SDC30 == 1
    #include "SparkFun_SCD30_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_SCD30
    SCD30 airSensor;
  unsigned long SDC30_old = millis();
  unsigned long SDC30_Repeat = 30000;
#endif // c_SDC30