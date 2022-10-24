String reset_reason(RESET_REASON reason)
{
  switch (reason)
  {
  case 1:
    return ("POWERON_RESET (1,Vbat power on reset)");
    break;
  case 3:
    return ("SW_RESET (3,Software reset digital core)");
    break;
  case 4:
    return ("OWDT_RESET (4,Legacy watch dog reset digital core)");
    break;
  case 5:
    return ("DEEPSLEEP_RESET (5,Deep Sleep reset digital core)");
    break;
  case 6:
    return ("SDIO_RESET (6,Reset by SLC module, reset digital core)");
    break;
  case 7:
    return ("TG0WDT_SYS_RESET (7,Timer Group0 Watch dog reset digital core)");
    break;
  case 8:
    return ("TG1WDT_SYS_RESET (8,Timer Group1 Watch dog reset digital core)");
    break;
  case 9:
    return ("RTCWDT_SYS_RESET (9,RTC Watch dog Reset digital core)");
    break;
  case 10:
    return ("INTRUSION_RESET (10,Instrusion tested to reset CPU)");
    break;
  case 11:
    return ("TGWDT_CPU_RESET (11,Time Group reset CPU)");
    break;
  case 12:
    return ("SW_CPU_RESET (12,Software reset CPU)");
    break;
  case 13:
    return ("RTCWDT_CPU_RESET (13,RTC Watch dog Reset CPU)");
    break;
  case 14:
    return ("EXT_CPU_RESET (14,for APP CPU, reseted by PRO CPU)");
    break;
  case 15:
    return ("RTCWDT_BROWN_OUT_RESET (15,Reset when the vdd voltage is not stable)");
    break;
  case 16:
    return ("RTCWDT_RTC_RESET (16,RTC Watch dog reset digital core and rtc module)");
    break;
  default:
    return (String(reason) + ",NO_MEAN");
  }
}