#if c_DS18B20 == 1
  #include <OneWire.h>
  #include <DallasTemperature.h>
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sens18b20(&oneWire);
  String st_DS18B20;
  unsigned long DS18B20_old = millis();
  unsigned long DS18B20_Repeat = 20000;
  RunningMedian RootTempRM = RunningMedian(10);  
#endif