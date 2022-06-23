
void syslog_ng(String x){

#if c_syslog == 1
syslog.log(LOG_INFO, fFTS(float(millis())/1000, 3) + "s " + x);
vTaskDelay(1);
#endif //c_syslog
}


void syslog_err(String x){

#if c_syslog == 1
syslog.log(LOG_ERR, fFTS(float(millis())/1000, 3) + "s " + x);
vTaskDelay(1);
#endif //c_syslog

}
