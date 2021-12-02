#define INIT_DYNAMIC_VAR(var, val, gfunc, afunc)   \
    do                                             \
    { SHELL_VAR *v = bind_variable(var, (val), 0); \
      if (v)                                       \
      {                                            \
          v->dynamic_value = gfunc;                \
          v->assign_func   = afunc;                \
      }                                            \
    }                                              \
    while (0)

static SHELL_VAR * assign_var (
     SHELL_VAR *self,
     char *value,
     arrayind_t unused,
     char *key ) {
  return (self);
}


long currentTimeMillis() {
  struct timeval time;
  gettimeofday(&time, NULL);

  return time.tv_sec * 1000 + time.tv_usec / 1000;
}

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
/*
char * init_dynamic_var(char *var_name, SHELL_VAR * (*fxn)(SHELL_VAR *)){
    char *msg;
    fprintf(msg, "var: %s, ", var_name);
    fprintf(stderr, "%s\n", msg);
    INIT_DYNAMIC_VAR (var_name, (char *)NULL, fxn, assign_var);
    return msg;
}
*/
int init_dynamic1(){
    INIT_DYNAMIC_VAR ("DYNAMIC1", (char *)NULL, get_dynamic1, assign_var);
    INIT_DYNAMIC_VAR ("ANSIBLE_FACTS", (char *)NULL, get_ansible_facts, assign_var);
    return 0;
}



//    INIT_DYNAMIC_VAR ("DYNAMIC1", (char *)NULL, get_ms, assign_var);
//
//  kill_all_local_variables();
//  unbind_variable("TS");
