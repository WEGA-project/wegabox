
void lcd(String x){
      #if c_LCD == 1
       oled.clear();
       
       oled.home();
        oled.println(x); 
        oled.update();    
        vTaskDelay(10); 
      #endif // c_LCD
}