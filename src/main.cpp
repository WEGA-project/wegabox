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
GMedian<7, int> PhMediana;
GMedian<3, int> DstMediana;    
GABfilter PhGAB(0.001, 150, 1);
GABfilter DstGAB(0.01, 150, 1);

#include <pre.h>
#include <func>

// Переменные
float AirTemp, AirHum, AirPress, RootTemp, CO2, tVOC,hall,pHmV,pHraw,NTC,Ap,An,Dist,PR;
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
  #include <Adafruit_AHTX0.h>
  Adafruit_AHTX0 aht;
  Adafruit_Sensor *aht_humidity, *aht_temp;
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
#endif

#if c_NTC == 1
 #define NTC_port 32
 #define NTC_MiddleCount 100000
#endif

#if c_EC == 1
  #define EC_DigitalPort1 18
  #define EC_DigitalPort2 19
  #define EC_AnalogPort 33
  #define EC_MiddleCount 60000
#endif

#if c_US025 == 1
  #define US_ECHO 13
  #define US_TRIG 14
  #define US_MiddleCount 6000

#endif // c_US025

#if c_MCP23017 == 1
  #include <Adafruit_MCP23X17.h>
  Adafruit_MCP23X17 mcp;
#endif // c_MCP23017

#if c_PR == 1
 #define PR_AnalogPort 35
 #define PR_MiddleCount 100
#endif // c_PR

#if c_BME280 == 1
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
#endif //c_BME280


#include <tasks.h>

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


  server.send(200, "text/html",  httpstr);
  }

#include <setup.h>

void loop() {


  #if c_DS18B20 == 1
    sens18b20.begin();
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
    sensors_event_t humidity;
    sensors_event_t temp;
    aht_humidity->getEvent(&humidity);
    aht_temp->getEvent(&temp);

    AirTemp=temp.temperature;
    AirHum=humidity.relative_humidity;
    
  #endif


  #if c_MCP3421 == 1
    long value = 0;
    MCP342x::Config status;
    // Initiate a conversion; convertAndRead() will wait until it can be read
    uint8_t err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot,MCP342x::resolution18, MCP342x::gain4,1000000, value, status);
    if (err) {
      Serial.print("Convert error: ");
      Serial.println(err);
    }
    else {
      pHraw=PhMediana.filtered(value);  // Медианная фильтрация удаляет резкие выбросы показаний
      if (millis() < 60000){            // Игнорит ошибку фильтра на старте системы первые 60 сек. 
        PhGAB.setParameters(10,10,10);
        PhGAB.filtered(pHraw);
        //pHmV=4096/pow(2,18)*pHraw/1;
      }else{
        PhGAB.setParameters(0.001, 200, 1);
        pHmV=4096/pow(2,18)*PhGAB.filtered(pHraw)/4;
      }
    }
  #endif // c_MCP3421

  #if c_ADS1115 == 1
    adc.setCompareChannels(ADS1115_COMP_0_3);
    pHraw=adc.getResult_mV();
    if (millis() < 60000){
      PhGAB.setParameters(1,1,1);
      PhGAB.filtered(pHraw);
      //pHmV=pHraw;
    }else{
      PhGAB.setParameters(0.001, 200, 1);
      pHmV=PhGAB.filtered(pHraw);
    }
  #endif // c_ADS1115

  #if c_MCP23017 == 1
  
    mcp.pinMode(0,OUTPUT);
    mcp.pinMode(1,OUTPUT);
    
    mcp.digitalWrite(0,HIGH);
    mcp.digitalWrite(1,LOW);
    delay (2000);
    mcp.digitalWrite(0,LOW);

  #endif // c_MCP23017

  #if c_PR == 1
  pinMode(PR_AnalogPort, INPUT);
   float PR0=0;
   double prcount = 0;
   while (prcount < PR_MiddleCount){
    prcount++;
    PR0=analogRead(PR_AnalogPort)+PR0;
   }
   PR=PR0/PR_MiddleCount;
  #endif // c_PR

  #if c_AM2320 == 1
    int status = AM2320.read();
    switch (status)
    {
      case AM232X_OK:
        AirHum=AM2320.getHumidity();
        AirTemp=AM2320.getTemperature();
        break;
      default:
        Serial.println(status);
      break;
    }
  #endif

  #if c_BME280 == 1
    bme.takeForcedMeasurement();
    AirTemp=bme.readTemperature();
    AirHum=bme.readHumidity();
    AirPress=bme.readPressure();
  #endif //c_BME280

} // loop






