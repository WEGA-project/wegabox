#if c_syslog == 1
    //Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, HOSTNAME, Firmware, LOG_KERN);

  syslog.server(SYSLOG_SERVER, SYSLOG_PORT);
  syslog.deviceHostname(HOSTNAME);
  syslog.appName(":");
  //syslog.defaultPriority(LOG_INFO);
#endif //c_syslog