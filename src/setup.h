// В этом файле заданы начальные параметры применяющиеся при включении устройство разово


void setup() {

   if ( xI2CSemaphore == NULL )
   {
      // Создание мьютексного семафора, который мы будем использовать
      // для обслуживания доступа к I2C:
      xI2CSemaphore = xSemaphoreCreateMutex();
      if ( ( xI2CSemaphore ) != NULL )
      {
         // Если семафор создан, то делаем его доступным (Give)
         // для использования при доступе к порту I2C:
         xSemaphoreGive( ( xI2CSemaphore ) );
      }
   }

  #if c_EC == 1
      pinMode(EC_DigitalPort1, INPUT);
      pinMode(EC_DigitalPort2, INPUT);
      pinMode(EC_AnalogPort, INPUT);      
  #endif // c_EC

  #if c_NTC == 1
    pinMode(NTC_port, INPUT);
  #endif // c_NTC  

  Serial.begin(9600);
  

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setHostname( HOSTNAME );

  ArduinoOTA
    .onStart([]() {
      OtaStart = true;
      
      #if c_EC == 1
          pinMode(EC_DigitalPort1, INPUT);
          pinMode(EC_DigitalPort2, INPUT);
          pinMode(EC_AnalogPort, INPUT);      
      #endif // c_EC

      #if c_NTC == 1
        pinMode(NTC_port, INPUT);
      #endif // c_NTC  

      
    
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });


  ArduinoOTA.begin();
  
  while (millis() < 30000)  ArduinoOTA.handle(); // Ожидание возможности прошивки сразу после включения до запуска всего остального

// Сканирование устройств на шине i2c  
    Wire.begin(I2C_SDA, I2C_SCL);

     I2CScanner scanner;
     scanner.Init();
     scanner.Scan();




  MDNS.begin( HOSTNAME );
  MDNS.addService("http", "tcp", 80);
  server.on("/",handleRoot);
  server.on("/reset",handleReset);
  server.on("/status",handleStatus);
  server.begin();


#include <dev/ds18b20/setup.h>
#include <dev/aht10/setup.h>
#include <dev/ads1115/setup.h>
#include <dev/ec/setup.h>
#include <dev/pr/setup.h>
#include <dev/us025/setup.h>
#include <dev/ccs811/setup.h>
#include <dev/am2320/setup.h>
#include <dev/mcp3421/setup.h>
#include <dev/bmp280/setup.h>
#include <dev/mcp23017/setup.h>
#include <dev/hx710b/setup.h>







xTaskCreate(TaskOTA,"TaskOTA",10000,NULL,0,NULL);
xTaskCreate(TaskWegaApi,"TaskWegaApi",10000,NULL,0,NULL);



#if c_EC == 1
xTaskCreate(TaskEC,"TaskEC",10000,NULL,2,NULL);
#endif

#if c_NTC == 1
//xTaskCreate(TaskNTC,"TaskNTC",10000,NULL,1,NULL);
#endif


#if c_PR == 1
xTaskCreate(TaskPR,"TaskPR",10000,NULL,0,NULL);
#endif // c_PR


#if c_US025 == 1
xTaskCreate(TaskUS,"TaskUS",10000,NULL,0,NULL);
#endif // c_PR

#if c_hall == 1
xTaskCreate(TaskHall,"TaskHall",10000,NULL,0,NULL);
#endif // c_hall

#if c_CPUTEMP == 1
xTaskCreate(TaskCPUtemp,"TaskCPUtemp",10000,NULL,0,NULL);
#endif // c_CPUTEMP



#if c_MCP3421 == 1
xTaskCreate(TaskMCP3421,"TaskMCP3421",10000,NULL,0,NULL);
#endif // c_MCP3421

#if c_CCS811 == 1
xTaskCreate(TaskCCS811,"TaskCCS811",10000,NULL,0,NULL);
#endif // c_CCS811


#if c_ADS1115 == 1
xTaskCreate(TaskADS1115,"ADS1115",10000,NULL,0,NULL);
#endif // c_ADS1115

#if c_AM2320 == 1
xTaskCreate(TaskAM2320,"AM2320",10000,NULL,0,NULL);
#endif // c_AM2320

#if c_BME280 == 1
xTaskCreate(TaskBME280,"BME280",10000,NULL,0,NULL);
#endif // c_BME280



#if c_AHT10 == 1
xTaskCreate(TaskAHT10,"TaskAHT10",10000,NULL,0,NULL);
#endif // c_TaskAHT10


}

