#if c_hall == 1

  xTaskCreate(TaskHall, "TaskHall", 10000, NULL, 0, NULL);


#endif //c_hall