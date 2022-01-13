#if c_CCS811 == 1
  // Enable CCS811
ccs811.set_i2cdelay(50); 
bool ok= ccs811.begin();
  

// Check if flashing should be executed
if( ccs811.application_version()==0x2000 ) { Serial.println("init: already has 2.0.0");} else
{
// Flash
//Serial.print("setup: starting flash of '");
//Serial.print(image_name);
//Serial.println("' in 5 seconds");
delay(5000);
//Serial.println("");
ok= ccs811.flash(image_data, sizeof(image_data));
if( !ok ) Serial.println("setup: CCS811 flash FAILED");
//Serial.println("");
}
ccs811.start(CCS811_MODE_1SEC);

xTaskCreate(TaskCCS811,"TaskCCS811",10000,NULL,0,&appTasks[appTaskCount++]);
#endif // c_CCS811