int dur_demo(){
  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  sleep(2);
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  if (end.tv_nsec < start.tv_nsec)
  {
    end.tv_nsec += 1000000000;
    end.tv_sec--;
  }
  printf("Delta time: [%ld.%09ld]s\n", (long)(end.tv_sec - start.tv_sec), (end.tv_nsec - start.tv_nsec));
  return(0);
}
