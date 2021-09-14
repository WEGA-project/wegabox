
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
WebServer server(80);

#include <WiFiClient.h>
#include <HTTPClient.h>
#include <pre.h>

#include <func>

// Переменные
float AirTemp, AirHum, RootTemp, CO2, tVOC,seta,hall,pHmV;


#define HOSTNAME "WEGABOX" // Имя системы и DDNS .local
#define ONE_WIRE_BUS 23    // Порт 1-Wire
#include <Wire.h>          // Шина I2C
#define I2C_SDA 21         // SDA
#define I2C_SCL 22         // SCL
 
#define DS18B20 1 
#define c_AHT10 1
#define c_AM2320 0
#define c_CCS811 1
#define c_hall 1
#define c_MCP3421 1


#if DS18B20 == 1
  #include <OneWire.h>
  #include <DallasTemperature.h>
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sens18b20(&oneWire);
#endif

#if c_AHT10 == 1
  #include <AHT10.h>
  AHT10 myAHT10(AHT10_ADDRESS_0X38);
#endif


#if c_CCS811 == 1
  #include "ccs811.h"  // CCS811 library
  CCS811 ccs811;
#endif

#if c_MCP3421 == 1
  #include <MCP342x.h>
// 0x68 is the default address for all MCP342x devices
  uint8_t address = 0x68;
  MCP342x adc = MCP342x(address);
  MCP342x::Config config(MCP342x::channel1, MCP342x::oneShot, MCP342x::resolution18, MCP342x::gain1);
  MCP342x::Config status;
  bool startConversion = false;
#endif


void handleRoot() {
  String httpstr="<meta http-equiv='refresh' content='10'>";
       httpstr += "HOSTNAME=" + String(HOSTNAME) + "<br>";
       
       if(RootTemp) { httpstr +=  "RootTemp=" + fFTS(RootTemp,3) + "<br>"; }
       if(AirTemp)  { httpstr +=  "AirTemp=" +  fFTS(AirTemp,3) + "<br>"; }
       if(AirHum)   { httpstr +=  "AirHum=" +   fFTS(AirHum,3) + "<br>"; }
       if(CO2)   { httpstr +=  "CO2=" +   fFTS(CO2,3) + "<br>"; }
       if(tVOC)   { httpstr +=  "tVOC=" +   fFTS(tVOC,3) + "<br>"; }
       if(hall)   { httpstr +=  "hall=" +   fFTS(hall,3) + "<br>"; }     
       if(pHmV)   { httpstr +=  "pHmV=" +   fFTS(pHmV,3) + "<br>"; }

  server.send(200, "text/html",  httpstr);
  }

void TaskOTA(void * parameters){
  for(;;){
    server.handleClient();
    ArduinoOTA.handle();
  }
}


void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setHostname( HOSTNAME );
  ArduinoOTA.begin();
  MDNS.begin( HOSTNAME );
  MDNS.addService("http", "tcp", 80);
  server.on("/",handleRoot);
  server.begin();

  #if DS18B20 == 1
      sens18b20.begin();
      sens18b20.setResolution(12);
  #endif

  #if c_AHT10 == 1
  Wire.begin(I2C_SDA, I2C_SCL);
  myAHT10.begin();
  #endif

  #if c_CCS811 == 1
    // Enable CCS811
  ccs811.set_i2cdelay(50); 
  ccs811.begin();
  ccs811.start(CCS811_MODE_1SEC);
  #endif

  #if c_MCP3421 == 1
    MCP342x::generalCallReset();
    delay(1); // MC342x needs 300us to settle
      // Check device present
    Wire.requestFrom(address, (uint8_t)1);
    if (!Wire.available()) {
      Serial.print("No device found at address ");
      Serial.println(address, HEX);
      while (1);
  }

  // Configure the device with the desired settings. If there are
  // multiple devices you must do this for each one.
  adc.configure(config);
  
  // First time loop() is called start a conversion
  startConversion = true;
  #endif


  server.handleClient();
  ArduinoOTA.handle();

  xTaskCreate(TaskOTA,"TaskOTA",10000,NULL,3,NULL);

}

void loop() {


  #if DS18B20 == 1
    sens18b20.requestTemperatures();
    float ds0=sens18b20.getTempCByIndex(0);
    if(ds0 != -127 and ds0 !=85) RootTemp=ds0; 
  #endif

  #if c_CCS811 == 1
      // Read
    ccs811.set_envdata_Celsius_percRH(AirTemp,AirHum);
    uint16_t eco2, etvoc, errstat, raw;
    ccs811.read(&eco2,&etvoc,&errstat,&raw); 
    
    // Print measurement results based on status
    if( errstat==CCS811_ERRSTAT_OK ) { 
      CO2=eco2;
      tVOC=etvoc;
    } 
  #endif


  #if c_AHT10 == 1
    AirTemp=myAHT10.readTemperature();
    delay(1000);
    AirHum=myAHT10.readHumidity();
  #endif

  #if c_hall == 1
      long n=0;
      double sensorValue=0;
      while ( n< 10){
       n++;
       sensorValue = hallRead()+sensorValue;
       }
    hall=sensorValue/n;
  #endif

  #if c_MCP3421 == 1

      long value = 0;
  uint8_t err;

  if (startConversion) {
    MCP342x::generalCallConversion();
    startConversion = false;
  }
  
  err = adc.read(value, status);
  if (!err && status.isReady()) { 

    startConversion = true;
    
    pHmV=4096/pow(2,18)*value;

  }



  #endif

}






