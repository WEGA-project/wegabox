// В этом файле расположены функции выполняющиеся параллельно freeRTOC

void TaskOTA(void *parameters)
{
  for (;;)
  {
    server.handleClient();
    ArduinoOTA.handle();
    vTaskDelay(100);
  }
}

#include <etc/syslog/func.h>
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
#include <dev/DualBMx/tasks.h>
#include <dev/sdc30/tasks.h>
#include <dev/lcd/tasks.h>
#include <dev/cput/tasks.h>
#include <dev/hall/tasks.h>
#include <dev/vl6180x/tasks.h>
#include <dev/vl6180x_us/tasks.h>
#include <dev/vl53l0x_us/tasks.h>