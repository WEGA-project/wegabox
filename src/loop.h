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
    aht.begin();
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
  long lastOffDoser;

    int A = 0;
    int B = 1;
    int C = 2;
    int D = 3;
    long del = 300;
    int stap = 1;


                
      mcp.pinMode(A, OUTPUT);     
      mcp.pinMode(B, OUTPUT);     
      mcp.pinMode(C, OUTPUT);     
      mcp.pinMode(D, OUTPUT);  

  if (millis()-lastOffDoser > 30000 ) {
    for (int i=0; i<=500; i++){
    // enn
    mcp.digitalWrite(A, LOW);   
    mcp.digitalWrite(B, HIGH);   
    mcp.digitalWrite(C, HIGH);   
    mcp.digitalWrite(D, LOW);   
    delayMicroseconds(del);
    


    //twee
      mcp.digitalWrite(A, LOW);   
      mcp.digitalWrite(B, HIGH);   
      mcp.digitalWrite(C, LOW);   
      mcp.digitalWrite(D, HIGH);   
      delayMicroseconds(del);

    //drie
      mcp.digitalWrite(A, HIGH);   
      mcp.digitalWrite(B, LOW);   
      mcp.digitalWrite(C, LOW);   
      mcp.digitalWrite(D, HIGH);   
      delayMicroseconds(del);


    //vier
      mcp.digitalWrite(A, HIGH);   
      mcp.digitalWrite(B, LOW);   
      mcp.digitalWrite(C, HIGH);   
      mcp.digitalWrite(D, LOW);   
      delayMicroseconds(del);

      lastOffDoser=millis();
      }
      mcp.pinMode(A, LOW);   
      mcp.pinMode(B, LOW);   
      mcp.pinMode(C, LOW);   
      mcp.pinMode(D, LOW);   
      
      delay (10000);
      
  }


  #endif // c_MCP23017

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