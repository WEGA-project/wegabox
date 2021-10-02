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
  ArduinoOTA.begin();
  MDNS.begin( HOSTNAME );
  MDNS.addService("http", "tcp", 80);
  server.on("/",handleRoot);
  server.begin();

  #if c_DS18B20 == 1
      sens18b20.begin();
      sens18b20.setResolution(12);
  #endif

  #if c_AHT10 == 1
    if (!aht.begin()) {
      Serial.println("Failed to find AHT10/AHT20 chip");
      while (1) {
        delay(10);
      }
    }

    Serial.println("AHT10/AHT20 Found!");
    aht_temp = aht.getTemperatureSensor();
    aht_temp->printSensorDetails();

    aht_humidity = aht.getHumiditySensor();
    aht_humidity->printSensorDetails();
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
      while (1);
   }
    // adc.configure(config);
    // startConversion = true;
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
 
  server.handleClient();
  ArduinoOTA.handle();

  xTaskCreate(TaskOTA,"TaskOTA",10000,NULL,5,NULL);
  xTaskCreate(TaskWegaApi,"TaskWegaApi",10000,NULL,1,NULL);

  #if c_EC == 1
  xTaskCreatePinnedToCore(TaskEC,"TaskEC",10000,NULL,4,NULL,0);
  #endif

  #if c_NTC == 1
  xTaskCreate(TaskNTC,"TaskNTC",10000,NULL,0,NULL);
  #endif


  #if c_PR == 1
  xTaskCreate(TaskPR,"TaskPR",10000,NULL,0,NULL);
  #endif // c_PR


  #if c_US025 == 1
  xTaskCreatePinnedToCore(TaskUS,"TaskUS",10000,NULL,2,NULL,1);
  #endif // c_PR

  #if c_hall == 1
  xTaskCreate(TaskHall,"TaskHall",10000,NULL,1,NULL);
  #endif // c_hall

  #if c_BME280 == 1
    if (! bme.begin(0x77, &Wire)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

  #endif //c_BME280

}