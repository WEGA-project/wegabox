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