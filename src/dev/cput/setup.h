#if c_CPUTEMP == 1

  //configure RTC slow clock to internal oscillator, fast clock to XTAL divided by 4
  rtc_clk_slow_freq_set(RTC_SLOW_FREQ_RTC);
  rtc_clk_fast_freq_set(RTC_FAST_FREQ_XTALD4);
  
  //read CPU speed
  rtc_cpu_freq_config_t freq_config;
  rtc_clk_cpu_freq_get_config(&freq_config);
  //should be "0 -- 150000 -- 240", internal oscillator running at ~150kHz and CPU at 240 MHz
  //printf("%d -- %d -- %d\r\n", (int)rtc_clk_slow_freq_get(), rtc_clk_slow_freq_get_hz(), freq_config.freq_mhz);


  xTaskCreate(TaskCPUTEMP, "TaskCPUTEMP", 2000, NULL, 0, NULL);
#endif // c_CPUTEMP