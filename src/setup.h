// В этом файле заданы начальные параметры применяющиеся при включении устройство разово


void setup() {

   if ( xI2CSemaphore == NULL )
   {
      // Создание мьютексного семафора, который мы будем использовать
      // для обслуживания доступа к порту I2C:
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

#if c_AM2320 == 1
  if (! AM2320.begin() )
  {
    Serial.println("AM2320 Sensor not found");      
  }
#endif

#if c_CCS811 == 1
  // Enable CCS811
ccs811.set_i2cdelay(50); 
bool ok= ccs811.begin();
  

// Check if flashing should be executed
if( ccs811.application_version()==0x2000 ) { Serial.println("init: already has 2.0.0");} else
{
// Flash
//Serial.print("setup: starting flash of '");
//Serial.print(image_name);
//Serial.println("' in 5 seconds");
delay(5000);
//Serial.println("");
ok= ccs811.flash(image_data, sizeof(image_data));
if( !ok ) Serial.println("setup: CCS811 flash FAILED");
//Serial.println("");
}
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
  }

#endif



#if c_MCP23017 == 1
  if (!mcp.begin_I2C()) {
    Serial.println("Error.");
    while (1);
  }
#endif // c_MCP23017

#if c_BME280 == 1
  bme.begin(0x76, &Wire);
  //bme.begin();
#endif //c_BME280

#if c_BMP280 == 1
    if (!bmx280.begin())
      {
        Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
        while (1);
      }

      if (bmx280.isBME280())
        Serial.println("sensor is a BME280");
      else
        Serial.println("sensor is a BMP280");

      //reset sensor to default parameters.
      bmx280.resetToDefaults();

      //by default sensing is disabled and must be enabled by setting a non-zero
      //oversampling setting.
      //set an oversampling setting for pressure and temperature measurements. 
      bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
      bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

      //if sensor is a BME280, set an oversampling setting for humidity measurements.
      if (bmx280.isBME280())
        bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);

xTaskCreate(TaskBMP280,"BMP280",10000,NULL,0,NULL);  
#endif //c_BMP280

#if c_HX710B == 1
  if ( !air_press.init() )
  { 
    Serial.println(F("HX710B not Found !"));    
  }
xTaskCreate(TaskHX710B,"HX710B",10000,NULL,0,NULL);
#endif //c_HX710B



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

#if c_MCP23017 == 1
// задаём свойства ШИМ-сигнала
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
ledcSetup(ledChannel, 150000, resolution);
ledcAttachPin(PWD1, ledChannel);

xTaskCreate(TaskMCP23017,"MCP23017",10000,NULL,0,NULL);
#endif // c_MCP23017

#if c_AHT10 == 1
xTaskCreate(TaskAHT10,"TaskAHT10",10000,NULL,0,NULL);
#endif // c_TaskAHT10


}

