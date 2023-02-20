I2CScanner scanner;

void debug(byte addscan)
{
  switch (addscan)
  {
  case 0x48:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " ADS1115 - 4-channel 16-bit ADC");
    break;
  case 0x76:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " BMP280/BME280 - Temp/Barometric sensor");
    break;
  case 0x77:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " BMP280/BME280 - Temp/Barometric sensor");
    break;
  case 0x38:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " AHT10 Humidity and Temperature sensor");
    break;
  case 0x5b:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " CCS811 Volatile organics (VOC) and equivalent CO2 (eCO2) sensor");
    break;
  case 0x5c:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " AM2320 Humidity and Temperature sensor");
    break;
  case 0x68:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " MCP3421 18-Bit Analog-to-Digital Converter");
    break;
  case 0x20:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " MCP23017 I2C GPIO Expander Breakout has 16 GPIO");
    break;
  case 0x61:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " SCD-30 - NDIR CO2 Temperature and Humidity Sensor");
    break;
  case 0x3c:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " SSD1306 OLED Display");
    break;
  case 0x29:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " vl6180x laser distance sensor");
    break;

  default:
    syslog_ng("I2C found: 0x" + String(addscan, HEX) + " unknown device ");
  }
}