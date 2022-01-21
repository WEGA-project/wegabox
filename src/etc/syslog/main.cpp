#if c_syslog == 1
    #include <Syslog.h>
    #define SYSLOG_SERVER "192.168.237.111"
    #define SYSLOG_PORT 514

    
    WiFiUDP udpClient;
    Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, HOSTNAME, Firmware, LOG_KERN);
#endif //c_syslog