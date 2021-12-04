
int ANSIBLE_VIRTUALIZATION_TYPE_INTERVAL = 30;

int reproc_bind(int argc, const char **argv){
//  bind_variable_value
  return 0;
}
int reproc_demo(int argc, const char **argv){
  (void) argc;
  reproc_t *process = NULL;
  char *output = NULL;
  size_t size = 0;
  int r = REPROC_ENOMEM;

  process = reproc_new();
  if (process == NULL) {
    goto finish;
  }

  r = reproc_start(process, argv + 1, (reproc_options){ 0 });
  if (r < 0) {
    goto finish;
  }
  r = reproc_close(process, REPROC_STREAM_IN);
  if (r < 0) {
    goto finish;
  }
  for (;;) {
    uint8_t buffer[4096];
    r = reproc_read(process, REPROC_STREAM_OUT, buffer, sizeof(buffer));
    if (r < 0) {
      break;
    }
    size_t bytes_read = (size_t) r;
    char *result = realloc(output, size + bytes_read + 1);
    if (result == NULL) {
      r = REPROC_ENOMEM;
      goto finish;
    }
    output = result;
    memcpy(output + size, buffer, bytes_read);
    output[size + bytes_read] = '\0';
    size += bytes_read;
  }

  if (r != REPROC_EPIPE) {
    goto finish;
  }
  //printf("%s", output);
  r = reproc_wait(process, REPROC_INFINITE);
  if (r < 0) {
    goto finish;
  }

finish:
  char *vn = "ANSIBLE_VIRTUALIZATION_TYPE";
  if (r < 0) {
    fprintf(stderr, "err> %s\n", reproc_strerror(r));
  }
  SHELL_VAR *set_var_name = find_variable(vn);

    char sj[strlen(output)];
    char s[strlen(output)];
    strcpy(s, output);
    char* token = strtok(s, " ");
    int qty = 0;
    while (token) {
      if (qty>3){
        sprintf(sj, "%s%s", sj, token);
      }
      token = strtok(NULL, " ");
      qty++;
    }
   
  log_info("Setting var %s to %d bytes of ansible output", vn, strlen(sj));
  //log_debug("%s=%s", vn,sj);

//  bind_variable(vn, sj, 0);
  reproc_destroy(process);
  free(output);
  return abs(r);
}
int reproc_schedule_demo(int argc, const char **argv){
    pid_t pid;
    if ((pid = fork()) < 0) {
        return (-1);
    } else if (pid == 0) {
      for(;;){
        log_debug("Child PID %d> ppid %d", getpid(), getppid());
        reproc_demo(argc, argv);
        sleep(ANSIBLE_VIRTUALIZATION_TYPE_INTERVAL);
      }
    } else {
      log_debug("Parent PID %d> ppid %d", getpid(), getppid());
      sleep(5);
    }
  return 0;
}

