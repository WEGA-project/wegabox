#if c_ADS1115 == 1
  void TaskADS1115(void * parameters) {
    for(;;){
    if (OtaStart == true) {vTaskDelete( NULL );}else{
        
            
      //adc.reset();   
      //delay(100);
      //  adc.init();
      //  delay(100);
        //while (adc.isBusy());

  
       
      //   vTaskDelay(500 / portTICK_PERIOD_MS); 
      // adc.setMeasureMode(ADS1115_CONTINUOUS); 
      // adc.setCompareChannels(ADS1115_COMP_0_3);
      // adc.setVoltageRange_mV(ADS1115_RANGE_4096);
      // adc.setConvRate(ADS1115_8_SPS);
      
if ( xSemaphoreTake( xI2CSemaphore, ( TickType_t ) 5 ) == pdTRUE )      {
      //vTaskDelay(2000 / portTICK_PERIOD_MS);   
      pHraw=adc.getRawResult();

      long cont=0;
      double sensorValue=0;
      while ( cont < ADS1115_MiddleCount and OtaStart != true){
       cont++;
       
        sensorValue =  adc.getResult_mV()+sensorValue;
      }
      
      PhRM.add(sensorValue/cont);
      pHmV=PhRM.getMedian();
      
   
      xSemaphoreGive( xI2CSemaphore );      }
  vTaskDelay(1000 / portTICK_PERIOD_MS);     
    }
   
  }
}
#endif // c_ADS1115