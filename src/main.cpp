/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Устройство для контроля и управления работой гидропонной установки и процессом выращивания растений.    //
// Является частью проекта WEGA, https://github.com/wega_project  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Firmware "beta-0.5.180122"


#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
WebServer server(80);
#include <loop.h>

#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
    WiFiClient client;
    HTTPClient http;
    

#include <RunningMedian.h>
RunningMedian DstRM = RunningMedian(60);
RunningMedian PhRM = RunningMedian(90);
RunningMedian NTCRM = RunningMedian(90);
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

// #include "soc/rtc_wdt.h"
// #include "esp_int_wdt.h"
// #include "esp_task_wdt.h"

// Переменные
float AirTemp, AirHum, AirPress, RootTemp,hall,pHmV,pHraw,NTC,Ap,An,Dist,PR,CPUTemp,CO2, tVOC, eRAW;
float wNTC,wR2,wEC;
bool OtaStart = false;
bool ECwork = false;
bool USwork = false;


String wegareply;
String err_wegaapi_json;
String dt;
// Калибровочные значения для определения ЕС
float EC_R1, EC_R2_p1, EC_R2_p2;



TaskHandle_t appTasks[16];
uint8_t appTaskCount = 0;


#define ONE_WIRE_BUS 23    // Порт 1-Wire
#include <Wire.h>          // Шина I2C
#define I2C_SDA 21         // SDA
#define I2C_SCL 22         // SCL
#include "I2CScanner.h"


// syslog
#include <Syslog.h>
#define SYSLOG_SERVER "192.168.237.111"
#define SYSLOG_PORT 514

WiFiUDP udpClient;
Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, HOSTNAME, Firmware, LOG_KERN);


SemaphoreHandle_t xI2CSemaphore;
SemaphoreHandle_t xSemaphoreX = NULL;

#include <dev/ds18b20/main.cpp>
#include <dev/aht10/main.cpp>
#include <dev/ads1115/main.cpp>
#include <dev/pr/main.cpp>
#include <dev/us025/main.cpp>
#include <dev/ccs811/main.cpp>
#include <dev/am2320/main.cpp>
#include <dev/mcp3421/main.cpp>
#include <dev/bmp280/main.cpp>
#include <dev/mcp23017/main.cpp>
#include <dev/hx710b/main.cpp>
#include <dev/ec/main.cpp>
#include <dev/ntc/main.cpp>


#if c_CPUTEMP == 1
  extern "C" {     
    uint8_t temprature_sens_read(); 
  }
#endif //c_CPUTEMP


#include <tasks.h>
#include <httpserv.h>
#include <wegaapi.h>

#include <setup.h>









