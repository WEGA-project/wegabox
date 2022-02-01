#if c_HX710B == 1
  if ( !HX710B_Press.init() )
  { 
    syslog_ng("HX710B not Found !");    
  }
  // else
  // {
xTaskCreate(TaskHX710B,"TaskHX710B",15000,NULL,0,NULL);
syslog_ng("HX710B add Task");
//  }
#endif //c_HX710B
