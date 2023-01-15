#if c_vcc == 1
void Taskvcc(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
      vTaskDelete(NULL);
    vTaskDelay(1000);

    unsigned long vcc_LastTime = millis() - vcc_old;

    if (xSemaphoreX != NULL and vcc_LastTime > vcc_Repeat)
    {
      if (xSemaphoreTake(xSemaphoreX, (TickType_t)1) == pdTRUE)
      {

        unsigned long vcc_time = millis();
        syslog_ng("Vcc Start " + fFTS(vcc_LastTime - vcc_Repeat, 0) + "ms");

        long err = 0;
        unsigned cont = 0;
        unsigned long t = millis();

        int i = 0;
        do
        {
          vccRM.add(rom_phy_get_vdd33());
          i++;
        } while (i < 64);


        Vcc = vccRM.getAverage() / 2048 * VccAdj;
        // 0.0005045f * v + 0.3368f,3

        vcc_time = millis() - vcc_time;
        syslog_ng("Vcc=" + fFTS(Vcc, 3));

        syslog_ng("Vcc: Error/Count " + String(err) + "/" + String(cont));
        syslog_ng("Vcc: Highest= " + fFTS(vccRM.getHighest(), 1));
        syslog_ng("Vcc: Lowest= " + fFTS(vccRM.getLowest(), 1));
        syslog_ng("Vcc: VccRAW= " + String(rom_phy_get_vdd33()));

        syslog_ng("Vcc " + fFTS(vcc_time, 0) + "ms end.");

        vcc_old = millis();
        xSemaphoreGive(xSemaphoreX);
        syslog_ng("Vcc Semaphore Give");
      }
    }
  }
}

#endif // c_vcc