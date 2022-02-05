#if c_LCD == 1
void TaskLCD(void *parameters)
{

  for (;;)
  {
      if (OtaStart == true){
          oled.clear();
          oled.home();
          oled.setScale(5);
          oled.println("OTA");
          oled.update();
      vTaskDelete(NULL);}
  
  oled.clear();
  oled.home();
  oled.setScale(3);
  oled.println("UPTIME");
  oled.println(fFTS(millis()/1000,0));
  oled.update();
  vTaskDelay(1000);

if (Dist){
  oled.clear();
  oled.home();
  oled.setScale(3);
  oled.println("Dist");
  oled.println(fFTS(Dist,1));
  oled.update();
  vTaskDelay(1000);
}

if (AirTemp){
  oled.clear();
  oled.home();
  oled.setScale(3);
  oled.println("AirTemp");
  oled.println(fFTS(AirTemp,1));
  oled.update();
  vTaskDelay(1000);
}

if (AirHum){
  oled.clear();
  oled.home();
  oled.setScale(3);
  oled.println("AirHum");
  oled.println(fFTS(AirHum,1));
  oled.update();
  vTaskDelay(1000);
}

if (CO2){
  oled.clear();
  oled.home();
  oled.setScale(3);
  oled.println("CO2");
  oled.println(fFTS(CO2,0));
  oled.update();
  vTaskDelay(1000);
}

  }
}
#endif // c_LCD