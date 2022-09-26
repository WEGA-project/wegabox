/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Устройство для контроля и управления работой гидропонной установки и процессом выращивания растений.    //
// Является частью проекта WEGA, https://github.com/wega_project  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Firmware "beta-0.8.260922"


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
float AirTemp, AirHum, AirPress, RootTemp,hall,pHmV,pHraw,NTC,Ap,An,Dist,DstRAW,CPUTemp,CO2, tVOC, eRAW;
float wNTC,wR2,wEC,wpH;
float PR=-1;
bool OtaStart = false;
bool ECwork = false;
bool USwork = false;
int readGPIO,PWD1,PWD2;
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




#define ONE_WIRE_BUS 23    // Порт 1-Wire
#include <Wire.h>          // Шина I2C
#define I2C_SDA 21         // SDA
#define I2C_SCL 22         // SCL



// syslog
#include <etc/syslog/main.cpp>

//SemaphoreHandle_t xSemaphoreI2C= NULL;;

SemaphoreHandle_t xSemaphoreX = NULL;

#include <rom/rtc.h>

String reset_reason(RESET_REASON reason)
{
  switch ( reason)
  {
    case 1 : return ("POWERON_RESET (1,Vbat power on reset)");break;
    case 3 : return ("SW_RESET (3,Software reset digital core)");break; 
    case 4 : return ("OWDT_RESET (4,Legacy watch dog reset digital core)");break; 
    case 5 : return ("DEEPSLEEP_RESET (5,Deep Sleep reset digital core)");break; 
    case 6 : return ("SDIO_RESET (6,Reset by SLC module, reset digital core)");break; 
    case 7 : return ("TG0WDT_SYS_RESET (7,Timer Group0 Watch dog reset digital core)");break;
    case 8 : return ("TG1WDT_SYS_RESET (8,Timer Group1 Watch dog reset digital core)");break;
    case 9 : return ("RTCWDT_SYS_RESET (9,RTC Watch dog Reset digital core)");break;
    case 10 : return ("INTRUSION_RESET (10,Instrusion tested to reset CPU)");break;
    case 11 : return ("TGWDT_CPU_RESET (11,Time Group reset CPU)");break; 
    case 12 : return ("SW_CPU_RESET (12,Software reset CPU)");break;   
    case 13 : return ("RTCWDT_CPU_RESET (13,RTC Watch dog Reset CPU)");break;  
    case 14 : return ("EXT_CPU_RESET (14,for APP CPU, reseted by PRO CPU)");break;  
    case 15 : return ("RTCWDT_BROWN_OUT_RESET (15,Reset when the vdd voltage is not stable)");break;
    case 16 : return ("RTCWDT_RTC_RESET (16,RTC Watch dog reset digital core and rtc module)");break;
    default : return (String(reason)+",NO_MEAN");
  }
}

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



#include <tasks.h>
#include <httpserv.h>
#include <wegaapi.h>

#include "I2CScanner.h"

I2CScanner scanner;


void debug(byte addscan)
{
	// Serial.print("Found at 0x");
	// Serial.println(addscan, HEX);
  syslog_ng("I2C found: device 0x"+ String(addscan, HEX));
  if (addscan == 0x48) syslog_ng("I2C found: ADS1115 - 4-channel 16-bit ADC");
  if (addscan == 0x76) syslog_ng("I2C found: BMP280/BME280 - Temp/Barometric sensor");
  if (addscan == 0x38) syslog_ng("I2C found: AHT10 Humidity and Temperature sensor");
  if (addscan == 0x5b) syslog_ng("I2C found: CCS811 Volatile organics (VOC) and equivalent CO2 (eCO2) sensor");
  if (addscan == 0x5c) syslog_ng("I2C found: AM2320 Humidity and Temperature sensor");
  if (addscan == 0x68) syslog_ng("I2C found: MCP3421 18-Bit Analog-to-Digital Converter");
  if (addscan == 0x20) syslog_ng("I2C found: MCP23017 I2C GPIO Expander Breakout has 16 GPIO");
  if (addscan == 0x61) syslog_ng("I2C found: SCD-30 - NDIR CO2 Temperature and Humidity Sensor");
  if (addscan == 0x3c) syslog_ng("I2C found: SSD1306 OLED Display");
  if (addscan == 0x29) syslog_ng("I2C found: vl6180x laser distance sensor"); 
}



#include <setup.h>









