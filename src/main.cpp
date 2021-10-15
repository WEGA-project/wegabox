/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Устройство для контроля и управления работой гидропонной установки и процессом выращивания растений.    //
// Является частью проекта WEGA, https://github.com/wega_project  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
WebServer server(80);

#include <WiFiClient.h>
#include <HTTPClient.h>
#include "GyverFilters.h"
GMedian<17, int> PhMediana;
GMedian<7, long> DstMediana;    

GKalman CpuTempKalman(1, 0.0001);

GABfilter ApGAB(0.05, 1, 1);
GABfilter AnGAB(0.05, 1, 1);
GABfilter NTCGAB(0.05, 1, 1);
GABfilter PRGAB(1, 1, 1);
GABfilter DstGAB(1, 1, 1);
GABfilter HallGAB(0.001, 1, 1);
GABfilter PhGAB(0.001, 150, 1);

GMedian<254, long> ApMed;
GMedian<254, long> AnMed;
GMedian<254, long> NTCMed;




#include <pre.h>
#include <func>


// Переменные
float AirTemp, AirHum, AirPress, RootTemp, CO2, tVOC,hall,pHmV,pHraw,NTC,Ap,An,Dist,PR,CPUTemp;
bool OtaStart = false;
bool ECwork = false;
bool USwork = false;



TaskHandle_t TaskAHT10Handler;

#define ONE_WIRE_BUS 23    // Порт 1-Wire
#include <Wire.h>          // Шина I2C
#define I2C_SDA 21         // SDA
#define I2C_SCL 22         // SCL


#if c_DS18B20 == 1
  #include <OneWire.h>
  #include <DallasTemperature.h>
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sens18b20(&oneWire);
#endif

#if c_AHT10 == 1
  #include <AHT10.h>
  uint8_t readStatus = 0;
  AHT10 myAHT10(AHT10_ADDRESS_0X38);
#endif

#if c_AM2320 == 1
  #include <AM232X.h>
  AM232X AM2320;
#endif

#if c_CCS811 == 1
  #include "ccs811.h"  // CCS811 library
  CCS811 ccs811;
#endif

#if c_MCP3421 == 1
  #include <MCP342x.h>
  uint8_t address = 0x68;
  MCP342x adc = MCP342x(address);
#endif

#if c_ADS1115 == 1
  #include<ADS1115_WE.h> 
  #define I2C_ADDRESS 0x48
  ADS1115_WE adc = ADS1115_WE(I2C_ADDRESS);
  #define ADS1115_MiddleCount 5000
#endif

#if c_NTC == 1
 #define NTC_port 32
 #define NTC_MiddleCount 100000
#endif

#if c_EC == 1
  #define EC_DigitalPort1 18
  #define EC_DigitalPort2 19
  #define EC_AnalogPort 33
  #define EC_MiddleCount 600000
#endif

#if c_US025 == 1
  #define US_ECHO 13
  #define US_TRIG 14
  #include <HCSR04.h>
  UltraSonicDistanceSensor distanceSensor(US_ECHO, US_TRIG);
  #define US_MiddleCount 900

#endif // c_US025

#if c_MCP23017 == 1
  #include <Adafruit_MCP23X17.h>
  Adafruit_MCP23X17 mcp;
#endif // c_MCP23017

#if c_PR == 1
 #define PR_AnalogPort 35
 #define PR_MiddleCount 1000
#endif // c_PR

#if c_BME280 == 1
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
#endif //c_BME280

#if c_CPUTEMP == 1
  extern "C" {      
    uint8_t temprature_sens_read(); 
  }
#endif //c_CPUTEMP

#include <tasks.h>
#include <status.h>

void handleReset(){
  server.send(200, "text/plain",  "restart");
  delay(5000);
  ESP.restart();
  
}

void handleRoot() {
  String httpstr="<meta http-equiv='refresh' content='10'>";
       httpstr += "HOSTNAME=" + String(HOSTNAME) + "<br>";
       
       if(RootTemp) { httpstr +=  "RootTemp=" + fFTS(RootTemp,3) + "<br>"; }
       if(AirTemp)  { httpstr +=  "AirTemp=" +  fFTS(AirTemp,3) + "<br>"; }
       if(AirHum)   { httpstr +=  "AirHum=" +   fFTS(AirHum,3) + "<br>"; }
       if(CO2)   { httpstr +=  "CO2=" +   fFTS(CO2,0) + "<br>"; }
       if(tVOC)   { httpstr +=  "tVOC=" +   fFTS(tVOC,0) + "<br>"; }
       if(hall)   { httpstr +=  "hall=" +   fFTS(hall,3) + "<br>"; }     
       if(pHmV)   { httpstr +=  "pHmV=" +   fFTS(pHmV,3) + "<br>"; }
       if(NTC)   { httpstr +=  "NTC=" +   fFTS(NTC,3) + "<br>"; }
       if(Ap)   { httpstr +=  "Ap=" +   fFTS(Ap,3) + "<br>"; }
       if(An)   { httpstr +=  "An=" +   fFTS(An,3) + "<br>"; }
       if(Dist)   { httpstr +=  "Dist=" +   fFTS(Dist,3) + "<br>"; }
       if(PR)   { httpstr +=  "PR=" +   fFTS(PR,3) + "<br>"; }
       if(AirPress)   { httpstr +=  "AirPress=" +   fFTS(AirPress,3) + "<br>"; }
       if(CPUTemp)   { httpstr +=  "CPUTemp=" +   fFTS(CPUTemp,3) + "<br>"; }
       

  server.send(200, "text/html",  httpstr);
  }

#include <setup.h>
#include <loop.h>







