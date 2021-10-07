// В этом файле заданы начальные параметры применяющиеся при включении устройство разово


void setup() {

 
  Serial.begin(9600);
  Wire.begin(I2C_SDA, I2C_SCL);

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



  while (millis() < 10000)  ArduinoOTA.handle(); // Ожидание возможности прошивки сразу после включения
  
  
  MDNS.begin( HOSTNAME );
  MDNS.addService("http", "tcp", 80);
  server.on("/",handleRoot);
  server.begin();

  #if c_DS18B20 == 1
      sens18b20.begin();
      sens18b20.setResolution(12);
  #endif

  #if c_AHT10 == 1
    // if (!aht.begin()) {
    //   Serial.println("Failed to find AHT10/AHT20 chip");
    // }

    // Serial.println("AHT10/AHT20 Found!");
    // aht_temp = aht.getTemperatureSensor();
    // aht_temp->printSensorDetails();

    // aht_humidity = aht.getHumiditySensor();
    // aht_humidity->printSensorDetails();

  
     
   myAHT10.softReset();
   delay(50);
    myAHT10.begin();
    myAHT10.setNormalMode();
  #endif

  #if c_AM2320 == 1
    if (! AM2320.begin() )
    {
      Serial.println("AM2320 Sensor not found");      
    }
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
   }

  #endif

  #if c_ADS1115 == 1
   Wire.begin(I2C_SDA, I2C_SCL);
   adc.init();
   adc.setConvRate(ADS1115_16_SPS);
   adc.setVoltageRange_mV(ADS1115_RANGE_4096);
   adc.setMeasureMode(ADS1115_CONTINUOUS);
  #endif // c_ADS1115

  #if c_MCP23017 == 1
    if (!mcp.begin_I2C()) {
      Serial.println("Error.");
      while (1);
    }
  #endif // c_MCP23017

  #if c_BME280 == 1
  bme.begin();
  #endif //c_BME280


  xTaskCreate(TaskOTA,"TaskOTA",10000,NULL,4,NULL);
  xTaskCreate(TaskWegaApi,"TaskWegaApi",10000,NULL,1,NULL);

  #if c_EC == 1
  //xTaskCreatePinnedToCore(TaskEC,"TaskEC",10000,NULL,3,NULL,0);
  xTaskCreate(TaskEC,"TaskEC",10000,NULL,3,NULL);
  #endif

  #if c_NTC == 1
  xTaskCreate(TaskNTC,"TaskNTC",10000,NULL,1,NULL);
  #endif


  #if c_PR == 1
  xTaskCreate(TaskPR,"TaskPR",10000,NULL,0,NULL);
  #endif // c_PR


  #if c_US025 == 1
  xTaskCreate(TaskUS,"TaskUS",10000,NULL,2,NULL);
  #endif // c_PR

  #if c_hall == 1
  xTaskCreate(TaskHall,"TaskHall",10000,NULL,0,NULL);
  #endif // c_hall

  #if c_CPUTEMP == 1
  xTaskCreate(TaskCPUtemp,"TaskCPUtemp",10000,NULL,0,NULL);
  #endif // c_CPUTEMP


  #if c_AHT10 == 1
  xTaskCreate(TaskAHT10,"TaskAHT10",10000,NULL,0,NULL);
  #endif // c_TaskAHT10

  #if c_MCP3421 == 1
  xTaskCreate(TaskMCP3421,"TaskMCP3421",10000,NULL,0,NULL);
  #endif // c_MCP3421

  #if c_CCS811 == 1
  xTaskCreate(TaskCCS811,"TaskCCS811",10000,NULL,0,NULL);
  #endif // c_CCS811

  #if c_DS18B20 == 1
  xTaskCreate(TaskDS18B20,"TaskDS18B20",10000,NULL,0,NULL);
  #endif // c_DS18B20

  #if c_ADS1115 == 1
  xTaskCreate(TaskADS1115,"ADS1115",10000,NULL,0,NULL);
  #endif // c_ADS1115

  #if c_AM2320 == 1
  xTaskCreate(TaskAM2320,"AM2320",10000,NULL,0,NULL);
  #endif // c_AM2320
 
  #if c_BME280 == 1
  xTaskCreate(TaskBME280,"BME280",10000,NULL,0,NULL);
  #endif // c_BME280

}
