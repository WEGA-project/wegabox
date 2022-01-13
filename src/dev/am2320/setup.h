#if c_AM2320 == 1
  if (! AM2320.begin() )
  {
    Serial.println("AM2320 Sensor not found");      
  }
#endif
