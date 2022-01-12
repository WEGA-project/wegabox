#if c_US025 == 1
void TaskUS(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);

    else
    {
        float us = distanceSensor.measureDistanceCm(25);
        
        if (us > 1) {
        DstRM.add (us);
        Serial.println(us);
        Dist= DstRM.getMedian();
        }
        else  {
          Serial.print("US-025 Error us=");
          Serial.println(us);
          }
              
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}
#endif // c_US025