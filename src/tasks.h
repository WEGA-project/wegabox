// В этом файле расположены функции выполняющиеся параллельно freeRTOC

void TaskOTA(void * parameters){
  for(;;){
    server.handleClient();
    ArduinoOTA.handle();
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

#if c_hall == 1
void TaskHall(void * parameters) {
  for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{

    //hall=HallGAB.filtered( hallRead() );
    HallRM.add(hallRead());
    hall=HallRM.getMedian();
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}
}
#endif //c_hall

#if c_CPUTEMP == 1
void TaskCPUtemp(void * parameters) {
  for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
    
     CpuTempRM.add( (temprature_sens_read()- 32)/1.8);
     CPUTemp=CpuTempRM.getMedian();
     //CPUTemp=CpuTempKalman.filtered( ( CPUTemp0 - 32 )/1.8 );
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
  }
}
}
#endif //c_CPUTEMP



#if c_CCS811 == 1
void TaskCCS811(void *parameters)
{
  for (;;)
  {
    if (xSemaphoreTake(xI2CSemaphore, (TickType_t)5) == pdTRUE)
    {
      // Read
      ccs811.set_envdata_Celsius_percRH(AirTemp, AirHum);
      uint16_t eco2, etvoc, errstat, raw;
      ccs811.read(&eco2, &etvoc, &errstat, &raw);

      // Print measurement results based on status
      if (errstat == CCS811_ERRSTAT_OK)
      {
        CO2 = eco2;
        tVOC = etvoc;
        eRAW = raw;
      }
      
      xSemaphoreGive(xI2CSemaphore);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}
#endif

#if c_MCP3421 == 1
void TaskMCP3421(void *parameters)
{
  long adcvalue;
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    else
    {
      if (xSemaphoreTake(xI2CSemaphore, (TickType_t)5) == pdTRUE)
      {
        MCP342x::Config status;
        long phvalue;
        uint8_t err = adc.convertAndRead(MCP342x::channel1, MCP342x::oneShot, MCP342x::resolution18, MCP342x::gain4, 100000, phvalue, status);
        if (!err)
        {

          //pHraw = phvalue;
          //pHmV = PhMediana.filtered(4096 / pow(2, 18) * pHraw / 4);
          pHraw = phvalue;
          PhRM.add(phvalue);
          pHmV=(4096 / pow(2, 18) *PhRM.getMedian()/ 4);
        }
        xSemaphoreGive(xI2CSemaphore);
      }
    vTaskDelay(2000 / portTICK_PERIOD_MS);  
    }
  }
}
#endif // c_MCP3421




#if c_AM2320 == 1
void TaskAM2320(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);

    else
    {
      vTaskDelay(2000 / portTICK_PERIOD_MS);

      if (AM2320.begin())
      {

        int status = AM2320.read();
        switch (status)
        {
        case AM232X_OK:
          AirHum = AM2320.getHumidity();
          AirTemp = AM2320.getTemperature();
          break;
        default:
          Serial.println(status);
          break;
        }
      }
    }
  }
}
#endif //c_AM2320

#if c_BME280 == 1
  void TaskBME280(void * parameters) {
  for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{  
    vTaskDelay(freqdb*1000 / portTICK_PERIOD_MS);
    AirTemp=bme.readTemperature();
    AirHum=bme.readHumidity();
    AirPress=bme.readPressure()*0.00750062;
    }
  }
  }
#endif //c_BME280

#if c_BMP280 == 1
  void TaskBMP280(void *parameters)
  {
    for (;;)
    {
      if (OtaStart == true)
      {
        vTaskDelete(NULL);
      }
      else
      {

        if (xSemaphoreTake(xI2CSemaphore, (TickType_t)5) == pdTRUE)
        {

          if (!bmx280.measure())
          {
            Serial.println("could not start measurement, is a measurement already running?");
            return;
          }

          //wait for the measurement to finish
          do
          {
            delay(100);
          } while (!bmx280.hasValue());

          AirTemp = bmx280.getTemperature();
          AirPress = bmx280.getPressure64() * 0.00750063755419211;

          if (bmx280.isBME280()) // Если датчик BME280 еще и влажность определить
          {
            AirHum = bmx280.getHumidity();
          }

          xSemaphoreGive(xI2CSemaphore);
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
      }
    }
  }
#endif //c_BMP280

#if c_HX710B == 1
  void TaskHX710B(void *parameters)
  {
    for (;;)
    {
      if (OtaStart == true)
      {
        vTaskDelete(NULL);
      }
      else
      {
            uint32_t data_raw = 0;
            if ( air_press.read(&data_raw, 1000UL) == HX710B_OK )
            {
              Serial.print(F("Data raw of ADC is : "));
              Serial.println((unsigned long) data_raw);
              //Dist=DstMediana.filtered(data_raw);
              DstRM.add(data_raw);
              Dist=DstRM.getMedian();
              Serial.println(Dist);
            }
      
       
      //  unsigned long cont=0;
      //  uint32_t data_raw = 0;
      //  long Dist0=0;
      //  while ( cont < 100){
      //    if ( air_press.read(&data_raw, 1000UL) == HX710B_OK ){
      //  cont++;
      //  Dist0=data_raw+Dist0;
      //  }
      // }
      // Dist=float(Dist0/cont);
      // Serial.print(F("Data raw of ADC is : "));
      // Serial.println(Dist);
       }
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }

#endif //c_HX710B

#if c_MCP23017 == 1

  void TaskMCP23017(void *parameters)
  {
    for (;;)
    {
      if (OtaStart == true)
      {
        vTaskDelete(NULL);
      }
      else
      {


      //pinMode(PWD1, OUTPUT);


      digitalWrite(PWD1, HIGH);

      //pinMode(PWD2, OUTPUT);
      //digitalWrite(PWD2, HIGH);

      mcp.pinMode(DRV1_A, OUTPUT);
      //mcp.pinMode(DRV1_B, OUTPUT);
      //mcp.pinMode(DRV1_C, OUTPUT);
      //mcp.pinMode(DRV1_D, OUTPUT);

      mcp.digitalWrite(DRV1_A, HIGH);
      //Serial.println("DRV1_A START");
      long pw=255;
      Serial.println(pw);
      ledcWrite(0, pw);
        delay (500);

      // pw=2;
      // Serial.println(pw);
      // ledcWrite(0, pw);
      //   delay (10000);
     for (int pw=3;pw<200;pw++){
         Serial.println(pw);
       ledcWrite(0, pw);
       delay (1000);
      }

      // pw=20;
      // Serial.println(pw);
      // ledcWrite(0, pw);
      //   delay (5000);

      // pw=50;
      // Serial.println(pw);
      // ledcWrite(0, pw);
      //   delay (5000);

      // pw=100;
      // Serial.println(pw);
      // ledcWrite(0, pw);
      //   delay (5000);

      // pw=200;
      // Serial.println(pw);
      // ledcWrite(0, pw);
      //   delay (5000);

      pw=254;
      Serial.println(pw);
      ledcWrite(0, pw);
        delay (5000);
        
      // for (int pw=10;pw<254;pw++){
      //   Serial.println(pw);
      // ledcWrite(0, pw);
      // delay (50);
      //  }
      // Serial.println("DRV1_A STOP"); 
      // for (int pw=254;pw>10;pw--){
      //   Serial.println(pw);
      // ledcWrite(0, pw);
      // delay (50);
      
      }


      vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
  }

#endif // c_MCP23017


#include <dev/ds18b20/tasks.h>
#include <dev/aht10/tasks.h>
#include <dev/ads1115/tasks.h>
#include <dev/ec/tasks.h>
#include <dev/pr/tasks.h>
#include <dev/us025/tasks.h>