#if c_AM2320 == 1
void TaskAM2320(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);

    else
    {
      vTaskDelay(2000 / portTICK_PERIOD_MS);

      if (AM2320.begin())
      {

        int status = AM2320.read();
        switch (status)
        {
        case AM232X_OK:
          AirHum = AM2320.getHumidity();
          AirTemp = AM2320.getTemperature();
          break;
        default:
          Serial.println(status);
          break;
        }
      }
    }
  }
}
#endif //c_AM2320