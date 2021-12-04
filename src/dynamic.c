

char * get_dynamic1_val(){
  char *p;
  struct timeval tv = { 0 };
  if (gettimeofday(&tv, NULL) < 0) {
    builtin_error("Failed to get time of day: %m");
    return NULL;
  }
  long TS = currentTimeMillis();
  if (asprintf(&p, "%ld", TS) < 0) {
    builtin_error("Failed to get memory for time of day: %m");
    return NULL;
  }
  return p;
}

static SHELL_VAR * get_ansible_facts (SHELL_VAR *var) {
  char *json_data =
  "[{\"name\": \"abe lincoln\", \"age\": 21},"
  " {\"name\": \"barb\", \"age\": 56}]";

  VSETATTR(var, att_exported);
  var_setvalue(var, json_data);
  return var;
}

static SHELL_VAR * get_dynamic1 (SHELL_VAR *var) {
  VSETATTR(var, att_integer);
  var_setvalue(var, get_dynamic1_val());
  return var;
}

int init_dynamic1(){
    INIT_DYNAMIC_VAR ("DYNAMIC1", (char *)NULL, get_dynamic1, assign_var);
    INIT_DYNAMIC_VAR ("ANSIBLE_FACTS", (char *)NULL, get_ansible_facts, assign_var);
    return 0;
}



