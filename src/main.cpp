/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Устройство для контроля и управления работой гидропонной установки и процессом выращивания растений.    //
// Является частью проекта WEGA, https://github.com/wega_project
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Firmware "beta-0.10.040123"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
WebServer server(80);
#include <Preferences.h>
Preferences preferences;

#include <loop.h>

#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
WiFiClient client;
HTTPClient http;

#include <RunningMedian.h>
RunningMedian PhRM = RunningMedian(90);

RunningMedian AirTempRM = RunningMedian(30);
RunningMedian AirHumRM = RunningMedian(30);
RunningMedian AirPressRM = RunningMedian(30);
RunningMedian PRRM = RunningMedian(30);

#include <pre.h>
#include <func>
#include <driver/adc.h>

// #include "soc/rtc_wdt.h"
// #include "esp_int_wdt.h"
//#include "esp_task_wdt.h"

// Переменные
float AirTemp, AirHum, AirPress, RootTemp, hall, pHmV, pHraw, NTC, Ap, An, Dist, DstRAW, CPUTemp, CO2, tVOC, eRAW;
float wNTC, wR2, wEC, wpH;
float PR = -1;
bool OtaStart = false;
bool ECwork = false;
bool USwork = false;
int readGPIO, PWD1, PWD2;
long ECStabOn;

String wegareply;
String err_wegaapi_json;
String dt;
String Reset_reason0, Reset_reason1;
// Калибровочные значения для определения ЕС
float EC_R1, EC_R2_p1, EC_R2_p2;

//#define SPIRAM_MALLOC_RESERVE_INTERNAL 0;

TaskHandle_t appTasks[48];
uint8_t appTaskCount = 0;

#define ONE_WIRE_BUS 23 // Порт 1-Wire
#include <Wire.h>       // Шина I2C
#define I2C_SDA 21      // SDA
#define I2C_SCL 22      // SCL

// syslog
#include <etc/syslog/main.cpp>

// SemaphoreHandle_t xSemaphoreI2C= NULL;;

SemaphoreHandle_t xSemaphoreX = NULL;

#include <rom/rtc.h>
#include <etc/ResetReason.h>


#include <dev/hall/main.cpp>
#include <dev/cput/main.cpp>
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
#include <dev/DualBMx/main.cpp>
#include <dev/sdc30/main.cpp>
#include <dev/lcd/main.cpp>
#include <dev/lcd/func.h>
#include <dev/vl6180x/main.cpp>
#include <dev/vl6180x_us/main.cpp>
#include <dev/vl53l0x_us/main.cpp>
#include <dev/doser/main.cpp>

#include <tasks.h>
#include <httpserv.h>
#include <wegaapi.h>

#include "I2CScanner.h"
#include <etc/I2Cdetect.h>


#include <setup.h>
