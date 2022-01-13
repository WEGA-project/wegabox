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