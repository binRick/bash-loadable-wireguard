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
