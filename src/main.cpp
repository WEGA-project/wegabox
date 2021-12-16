/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Устройство для контроля и управления работой гидропонной установки и процессом выращивания растений.    //
// Является частью проекта WEGA, https://github.com/wega_project  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Firmware "beta-0.3.131221"


#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
WebServer server(80);

#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "GyverFilters.h"
GMedian<17, int> PhMediana;
GMedian<7, long> DstMediana;  
GMedian<7, float> AirTempMediana;  
GMedian<7, float> AirHumMediana;
GMedian<7, float> RootTempMediana;
GMedian<60, float> PRMediana;

GKalman CpuTempKalman(1, 0.0001);

GABfilter ApGAB(10, 1, 1);
GABfilter AnGAB(10, 1, 1);
GABfilter NTCGAB(10, 1, 1);
GABfilter PRGAB(1, 1, 1);
GABfilter DstGAB(1, 1, 1);
GABfilter HallGAB(0.001, 1, 1);
GABfilter PhGAB(0.001, 150, 1);

GMedian<254, long> ApMed;
GMedian<254, long> AnMed;
GMedian<254, long> NTCMed;


#include <pre.h>
#include <func>
#include <driver/adc.h>

#include "soc/rtc_wdt.h"
#include "esp_int_wdt.h"
#include "esp_task_wdt.h"

// Переменные
float AirTemp, AirHum, AirPress, RootTemp,hall,pHmV,pHraw,NTC,Ap,An,Dist,PR,CPUTemp,CO2, tVOC, eRAW;
float wNTC,wR2,wEC;
bool OtaStart = false;
bool ECwork = false;
bool USwork = false;

unsigned long t_EC=0;
//,t_Dist,t_NTC,t_pH;
float f_EC=0;
String wegareply;
String err_wegaapi_json;
String dt;
// Калибровочные значения для определения ЕС
float EC_R1, EC_R2_p1, EC_R2_p2;



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
  String st_DS18B20;
#endif

#if c_AHT10 == 1
  #include <AHT10.h>
  uint8_t readStatus = 0;
  AHT10 myAHT10(AHT10_ADDRESS_0X38);
  String st_AHT10;
#endif

#if c_AM2320 == 1
  #include <AM232X.h>
  AM232X AM2320;
  String st_AM2320;
#endif

#if c_CCS811 == 1
  #include "ccs811.h"  // CCS811 library
  #include <spec/CCS811_FW_App_v2-0-0.h>
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
 #define NTC_port ADC1_CHANNEL_4 // gpio32
 #define NTC_MiddleCount 100000
#endif

#if c_EC == 1
  #define EC_DigitalPort1 18
  #define EC_DigitalPort2 19
  #define EC_AnalogPort ADC1_CHANNEL_5 // gpio33
  #define EC_MiddleCount 500000  // 12000 в секунду
#endif

#if c_US025 == 1
  #define US_ECHO 13
  #define US_TRIG 14
  #include <HCSR04.h>
  UltraSonicDistanceSensor distanceSensor(US_ECHO, US_TRIG);
  #define US_MiddleCount 6000

#endif // c_US025

#if c_MCP23017 == 1
  #include <Adafruit_MCP23X17.h>
  Adafruit_MCP23X17 mcp;
#endif // c_MCP23017

#if c_PR == 1
 #define PR_AnalogPort ADC1_CHANNEL_7 // gpio35
 #define PR_MiddleCount 10000
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
#include <httpserv.h>
#include <wegaapi.h>
#include <setup.h>
#include <loop.h>







