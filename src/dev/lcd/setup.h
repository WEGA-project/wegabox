#if c_LCD == 1

  oled.init();              // инициализация
  
  oled.clear();
  oled.home();
  oled.autoPrintln(true);
  oled.setScale(3);
  oled.println("WEGABOX");
  oled.setScale(1.3);
  oled.println();
  oled.println(Firmware);
  oled.println(WiFi.localIP());
  oled.print(HOSTNAME);
  oled.println(".local");
  oled.update();

//xTaskCreate(TaskLCD,"TaskLCD",5000,NULL,0,NULL);
xTaskCreatePinnedToCore(TaskLCD,"TaskLCD",8000,NULL,0,NULL,0);
syslog_ng("LCD add Task");
#endif // c_LCD


