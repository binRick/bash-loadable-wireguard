
char *extract_stdout_json(char *output, char *sj){
    char s[strlen(output)+1];
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
  log_debug("sj=%d bytes", strlen(sj));
  return sj;
}

char *parse_ansible_adhoc_stdout_to_struct(char *stdout_content){
  struct ADHOC_RESULT { char *output; int exit_code; } adhoc_struct[1];
  adhoc_struct[0].output = stdout_content;
  adhoc_struct[0].exit_code = 9;
  log_debug("Ansible exited %d with %d bytes of stdout", adhoc_struct[0].exit_code, strlen(adhoc_struct[0].output));
  return "";
}

json_object_t *get_ansible_facts_json(char *hostname){
  reproc_t *process = NULL;
  char *output = NULL;
  size_t size = 0;
  int r = REPROC_ENOMEM;
  process = reproc_new();
  if (process == NULL) {
    goto finish;
  }
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  char hostname_comma[100];
  char module_name[100];
  char module_args[100];
  sprintf(module_args, 
    "%s"
    "\0", 
    "gather_subset=all"
  );
  sprintf(module_name, 
    "%s"
    "\0", 
    "setup"
  );
  sprintf(hostname_comma, 
    "%s,"
    "\0", 
    hostname
  );
  char const *ansible_argv[] = { 
    "/usr/bin/env", 
        "command", "ansible", 
          hostname, "-i", hostname_comma, 
          "--one-line",
            "-m", module_name, 
            "-a", module_args
  };
  char *envex[] = { 
    "ANSIBLE_TRANSPORT=local",
    "ANSIBLE_SKIP_TAGS=",
    "ANSIBLE_RUN_TAGS=",
    "ANSIBLE_VERBOSE_TO_STDERR=True",
    "ANSIBLE_INVENTORY_ENABLED=host_list,",
    "ANSIBLE_SSH_RETRIES=0",
    "ANSIBLE_VERBOSITY=1",
    "ANSIBLE_ERROR_ON_UNDEFINED_VARS=True",
    "ANSIBLE_NO_TARGET_SYSLOG=True",
    "ANSIBLE_REMOTE_USER=root",
    "ANSIBLE_STDOUT_CALLBACK=json",
    "ANSIBLE_TIMEOUT=5",
    "ANSIBLE_ERROR_ON_MISSING_HANDLER=True",
    "ANSIBLE_LOAD_CALLBACK_PLUGINS=False",
    "ANSIBLE_SYSTEM_WARNINGS=False",
    "ANSIBLE_RETRY_FILES_SAVE_PATH=False",
    "ANSIBLE_HOST_KEY_CHECKING=False",
    "ANSIBLE_DEPRECATION_WARNINGS=False",
    "ANSIBLE_DISPLAY_ARGS_TO_STDOUT=False",
    "ANSIBLE_ENABLE_TASK_DEBUGGER=False",
    "ANSIBLE_FORCE_COLOR=False",
    "DEFAULT_KEEP_REMOTE_FILES=False",
    "ANSIBLE_NOCOLOR=True",
    "ANSIBLE_NOCOWS=True",
    "ANY_ERRORS_FATAL=True",
    "ANSIBLE_PYTHON_INTERPRETER=auto_silent", 
    "\0"
  };
  r = reproc_start(process, ansible_argv, (reproc_options){ 
      .redirect.discard = false,
      .working_directory = "/",
//      .env.extra = envex,
      .deadline = 1500,
      .redirect.out.type = REPROC_REDIRECT_PIPE,
      .redirect.err.type = REPROC_REDIRECT_PIPE,
      .redirect.parent = false
  });
  if (r < 0) {
    goto finish;
  }
  r = reproc_close(process, REPROC_STREAM_IN);
  if (r < 0) {
    goto finish;
  }
  for (;;) {
    uint8_t buffer[4096], buffer1[4096];
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
  r = reproc_wait(process, REPROC_DEADLINE);
  if (r < 0) {
    goto finish;
  }

finish:
  int forked_pid = reproc_pid(process);
  const char *stderr_content = reproc_strerror(r);
  if (r < 0){
    log_debug("err> %s", stderr_content);
  }
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  if(end.tv_nsec < start.tv_nsec){
    end.tv_nsec += 1000000000;
    end.tv_sec--;
  }
  char msg[100];
  sprintf(msg, 
    ""
    "Ansible Ad Hoc PID %d Exited with %d bytes of stdout in %ld.%09lds"
    "\0",
    forked_pid,
    strlen(output),
    (long)(end.tv_sec - start.tv_sec), 
    (end.tv_nsec - start.tv_nsec)
  );
  log_info("msg> %s",msg);
  char j[strlen(output)];
  char _j[strlen(output)];
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
  json_t json;
  json_load(&json, sj);
  json_object_t *facts = json_get_object(json.root, "ansible_facts");

  char *ansible_architecture = json_get_string(facts, "ansible_architecture");
  char *ansible_distribution = json_get_string(facts, "ansible_distribution");
  char *ansible_distribution_version = json_get_string(json_get_object(json.root, "ansible_facts"), "ansible_distribution_version");
  char *ansible_domain = json_get_string(json_get_object(json.root, "ansible_facts"), "ansible_domain");
  char *ansible_fqdn = json_get_string(json_get_object(json.root, "ansible_facts"), "ansible_fqdn");
  char *ansible_kernel = json_get_string(json_get_object(json.root, "ansible_facts"), "ansible_kernel");
  char *ansible_pkg_mgr = json_get_string(json_get_object(json.root, "ansible_facts"), "ansible_pkg_mgr");
  char *ansible_python_version = json_get_string(json_get_object(json.root, "ansible_facts"), "ansible_python_version");
  char *ansible_ssh_host_key_rsa_public = json_get_string(json_get_object(json.root, "ansible_facts"), "ansible_ssh_host_key_rsa_public");
  char *ansible_system = json_get_string(json_get_object(json.root, "ansible_facts"), "ansible_system");
  char *ansible_user_id = json_get_string(json_get_object(json.root, "ansible_facts"), "ansible_user_id");
  char *ansible_user_shell = json_get_string(json_get_object(json.root, "ansible_facts"), "ansible_user_shell");
  char *ansible_service_mgr = json_get_string(json_get_object(json.root, "ansible_facts"), "ansible_service_mgr");
  char dt[100];
  log_debug("Delta time: [%ld.%09ld]s" "\0", (long)(end.tv_sec - start.tv_sec), (end.tv_nsec - start.tv_nsec) );

  log_debug(""
    "\n** %s **"
    "\npid|forked:                               %d|%d"
    "\nansible_python_version:                   %s"
    "\nansible_pkg_mgr:                          %s"
    "\nansible_architecture:                     %s"
    "\nansible_distribution:                     %s %s"
    "\nfqdn|domain:                              %s|%s"
    "\nansible_kernel:                           %s"
    "\nansible_user_shell:                       %s",
    msg,
    getpid(), forked_pid,
    ansible_python_version,
    ansible_pkg_mgr,
    ansible_architecture,
    ansible_distribution, ansible_distribution_version,
    ansible_fqdn, ansible_domain,
    ansible_kernel,
    ansible_user_shell
  );
  reproc_destroy(process);
  free(output);
  return facts;
}
