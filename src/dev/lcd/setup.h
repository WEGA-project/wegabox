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
  //delay(5000);




//   display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
//   display.clearDisplay();
//   display.setTextSize(3);
//   display.setTextColor(SSD1306_WHITE);

// // // Start greeting 
  
//   display.setCursor(0, 0);  
//   display.println("WEGABOX");
//   display.setTextSize(1);
//   display.println(Firmware);
//   display.println("");
//   display.setTextSize(1.3);
//   display.println(WiFi.localIP());
//   display.print(HOSTNAME);
//   display.println(".local");
//   display.display(); // Show initial text
   
   
   
//    vTaskDelay(1000);

#endif // c_LCD


