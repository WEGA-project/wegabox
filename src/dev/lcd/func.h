
void lcd(String x){
      #if c_LCD == 1
       oled.clear();
       oled.home();
        oled.println(x); 
        oled.update();     
      #endif // c_LCD
}