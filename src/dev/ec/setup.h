
#if c_EC == 1

xTaskCreate(TaskEC,"TaskEC",500000,NULL,2,&appTasks[appTaskCount++]);
#endif
