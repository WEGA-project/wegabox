// В этом файле расположены функции выполняющиеся параллельно freeRTOC

void TaskOTA(void *parameters)
{
  for (;;)
  {
    server.handleClient();
    ArduinoOTA.handle();
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

#if c_hall == 1
void TaskHall(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
    {
      vTaskDelete(NULL);
    }
    else
    {

      HallRM.add(hallRead());
      hall = HallRM.getMedian();
      vTaskDelay(300 / portTICK_PERIOD_MS);
    }
  }
}
#endif //c_hall

#if c_CPUTEMP == 1
void TaskCPUtemp(void *parameters)
{
  for (;;)
  {
    if (OtaStart == true)
    {
      vTaskDelete(NULL);
    }
    else
    {

      CpuTempRM.add((temprature_sens_read() - 32) / 1.8);
      CPUTemp = CpuTempRM.getMedian();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}
#endif //c_CPUTEMP

#include <dev/ds18b20/tasks.h>
#include <dev/aht10/tasks.h>
#include <dev/ads1115/tasks.h>
#include <dev/pr/tasks.h>
#include <dev/us025/tasks.h>
#include <dev/ccs811/tasks.h>
#include <dev/am2320/tasks.h>
#include <dev/mcp3421/tasks.h>
#include <dev/bmp280/tasks.h>
#include <dev/mcp23017/tasks.h>
#include <dev/hx710b/tasks.h>
#include <dev/ec/tasks.h>
#include <dev/ntc/tasks.h>