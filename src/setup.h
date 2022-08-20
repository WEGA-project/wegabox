// В этом файле заданы начальные параметры применяющиеся при включении устройство разово

void setup()
{

#include <etc/syslog/setup.h>


#if c_EC == 1
  pinMode(EC_DigitalPort1, INPUT);
  pinMode(EC_DigitalPort2, INPUT);
  pinMode(EC_AnalogPort, INPUT);
#endif // c_EC

#if c_NTC == 1
  pinMode(NTC_port, INPUT);
#endif // c_NTC

  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setHostname(HOSTNAME);

  ArduinoOTA
      .onStart([]()
               {
                 OtaStart = true;
                 // for (uint8_t i = 0; i < appTaskCount; ++i) {
                 //   vTaskDelete(appTasks[i]);
                 //   syslog.log(LOG_INFO, "appTask" + fFTS(i,0) + " sec OTA");
                 //   //vTaskDelay(appTasks[i]);
                 // }

                 syslog_ng("OTA Start");

#if c_EC == 1
                 pinMode(EC_DigitalPort1, INPUT);
                 pinMode(EC_DigitalPort2, INPUT);
                 pinMode(EC_AnalogPort, INPUT);
#endif // c_EC

                 String type;
                 if (ArduinoOTA.getCommand() == U_FLASH)
                   type = "sketch";
                 else // U_SPIFFS
                   type = "filesystem";

                 // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                 Serial.println("Start updating " + type);
               })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
                 Serial.printf("Error[%u]: ", error);
                 if (error == OTA_AUTH_ERROR)
                   Serial.println("Auth Failed");
                 else if (error == OTA_BEGIN_ERROR)
                   Serial.println("Begin Failed");
                 else if (error == OTA_CONNECT_ERROR)
                   Serial.println("Connect Failed");
                 else if (error == OTA_RECEIVE_ERROR)
                   Serial.println("Receive Failed");
                 else if (error == OTA_END_ERROR)
                   Serial.println("End Failed");
               });

  syslog_ng("WEGABOX: Start system");
  syslog_ng("WEGABOX NAME: "+ String(HOSTNAME));
  syslog_ng("WEGABOX FW: "+ String(Firmware));






#include <dev/lcd/setup.h>
  ArduinoOTA.begin();

syslog_ng("WEGABOX: Wait OTA 30 sec");
  while (millis() < 30000)
    ArduinoOTA.handle(); // Ожидание возможности прошивки сразу после включения до запуска всего остального
Reset_reason0=reset_reason(rtc_get_reset_reason(0));
Reset_reason1=reset_reason(rtc_get_reset_reason(1));

syslog_ng("Reset_reason CPU0: "+ Reset_reason0 );
syslog_ng("Reset_reason CPU1: "+ Reset_reason1 );

  // Сканирование устройств на шине i2c
  Wire.begin(I2C_SDA, I2C_SCL);
  
  //Wire.begin();
  syslog_ng("I2C Wire Clock:"+String(Wire.getClock()));
  
syslog_ng("I2C: Scan I2C bus");
  I2CScanner scanner;
  scanner.Init();
  scanner.Scan();
  syslog_ng("I2C found devices:"+ String(scanner.Devices_Count));






  preferences.begin("settings", false); 

  MDNS.begin(HOSTNAME);
  MDNS.addService("http", "tcp", 80);
  //server.enableDelay(true);
  server.on("/", handleRoot);
  server.on("/menu", handleMenu);
  server.on("/main", handleMain);
  server.on("/reset", handleReset);
  server.on("/status", handleStatus);
  server.on("/pwd", handlePWD);
  server.on("/settings", handleSettings);
  server.on("/SettingsPomps", handleSettingsPomps);
  server.begin();

  http.setConnectTimeout(2000);
  http.setTimeout(1000);

  xTaskCreate(TaskOTA, "TaskOTA", 5000, NULL, 3, NULL);
  xTaskCreate(TaskWegaApi, "TaskWegaApi", 10000, NULL, 1, &appTasks[appTaskCount++]);

  xSemaphoreX = xSemaphoreCreateMutex();

  adc1_config_width(ADC_WIDTH_BIT_12);

#include <dev/ntc/setup.h>
#include <dev/ds18b20/setup.h>
#include <dev/aht10/setup.h>
#include <dev/us025/setup.h>
#include <dev/ccs811/setup.h>
#include <dev/am2320/setup.h>
#include <dev/mcp3421/setup.h>
#include <dev/bmp280/setup.h>
#include <dev/mcp23017/setup.h>
#include <dev/hx710b/setup.h>
#include <dev/ads1115/setup.h>
#include <dev/pr/setup.h>
#include <dev/ec/setup.h>
#include <dev/DualBMx/setup.h>
#include <dev/sdc30/setup.h>
#include <dev/cput/setup.h>
#include <dev/hall/setup.h>
#include <dev/vl6180x/setup.h>
#include <dev/vl6180x_us/setup.h>

#if c_LCD == 1
  oled.clear();
  //  oled.autoPrintln(true);
  //  oled.setScale(1);
  oled.update();

  //xTaskCreate(TaskLCD,"TaskLCD",20000,NULL,0,NULL);

#endif // c_LCD
}
