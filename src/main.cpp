/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Устройство для контроля и управления работой гидропонной установки и процессом выращивания растений.    //
// Является частью проекта WEGA, https://github.com/wega_project  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Firmware "beta-0.4.120122"


#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
WebServer server(80);

#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#include <RunningMedian.h>
RunningMedian DstRM = RunningMedian(60);
RunningMedian PhRM = RunningMedian(90);
RunningMedian NTCRM = RunningMedian(10);
RunningMedian RootTempRM = RunningMedian(10);

RunningMedian AirTempRM = RunningMedian(30);
RunningMedian AirHumRM = RunningMedian(30);
RunningMedian AirPressRM = RunningMedian(30);

RunningMedian PRRM = RunningMedian(30);

RunningMedian HallRM = RunningMedian(10);
RunningMedian CpuTempRM = RunningMedian(10);


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



//TaskHandle_t TaskAHT10Handler;

#define ONE_WIRE_BUS 23    // Порт 1-Wire
#include <Wire.h>          // Шина I2C
#define I2C_SDA 21         // SDA
#define I2C_SCL 22         // SCL
#include "I2CScanner.h"

SemaphoreHandle_t xI2CSemaphore;

#include <dev/ds18b20/main.cpp>
#include <dev/aht10/main.cpp>
#include <dev/ads1115/main.cpp>
#include <dev/ec/main.cpp>
#include <dev/pr/main.cpp>
#include <dev/us025/main.cpp>

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

#if c_MCP23017 == 1
  #include <Adafruit_MCP23X17.h>
  Adafruit_MCP23X17 mcp;
#endif // c_MCP23017

#if c_BME280 == 1
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
#endif //c_BME280

#if c_BMP280 == 1
// Arduino - BMP280 / BME280
// 3.3V ---- VCC
// GND ----- GND
// SDA ----- SDA
// SCL ----- SCL
// some BMP280/BME280 modules break out the CSB and SDO pins as well: 
// 5V ------ CSB (enables the I2C interface)
// GND ----- SDO (I2C Address 0x76)
// 5V ------ SDO (I2C Address 0x77)
// other pins can be left unconnected.

#include <Arduino.h>
#include <Wire.h>

#include <BMx280I2C.h>

//#define I2C_ADDRESS 0x76

//create a BMx280I2C object using the I2C interface with I2C Address 0x76
BMx280I2C bmx280(0x76);

#endif //c_BMP280



#if c_CPUTEMP == 1
  extern "C" {      
    uint8_t temprature_sens_read(); 
  }
#endif //c_CPUTEMP


#if c_HX710B == 1
  #include "HX710B.h"
  #define SCK_PIN 14
  #define SDI_PIN 13

  HX710B air_press(SCK_PIN, SDI_PIN);
  uint32_t time_update = 0;
#endif //c_HX710B

#if c_MCP23017 == 1
  #define PWD1 16
  #define PWD2 17
  #define DRV1_A 0
  #define DRV1_B 1
  #define DRV1_C 2
  #define DRV1_D 3

#endif // c_MCP23017

#include <tasks.h>
#include <httpserv.h>
#include <wegaapi.h>
#include <setup.h>
#include <loop.h>







