
//extern char **environ;
char *colors[] =
{
  "black",
  "red",
  "green",
  "yellow",
  "blue",
  "magenta",
  "cyan",
  "white"
};


void fgcolor(char *clr){
  unsigned int i;

  if (
    !strcmp(clr, "off")
     || !strcmp(clr, "reset")
     || !strcmp(clr, "clear")
    )
  {
    printf(FG_RESET);
    return;
  }
  else if (!strcmp(clr, "bd"))
  {
    printf(FG_BD);
    return;
  }
  else if (!strcmp(clr, "strike"))
  {
    printf(FG_STRIKE);
    return;
  }
  else if (!strcmp(clr, "inverse"))
  {
    printf(FG_INVERSE);
    return;
  }
  else if (!strcmp(clr, "faint"))
  {
    printf(FG_FAINT);
    return;
  }
  else if (
    !strcmp(clr, "invisible")
          || !strcmp(clr, "hide")
    )
  {
    printf(FG_INVISIBLE);
    return;
  }
  else if (!strcmp(clr, "rapidblink"))
  {
    printf(FG_RAPID_BLINK);
    return;
  }
  else if (!strcmp(clr, "blink"))
  {
    printf(FG_BLINK);
    return;
  }
  else if (!strcmp(clr, "ired"))
  {
    printf(FG_I_RED);
    return;
  }
  else if (!strcmp(clr, "iyellow"))
  {
    printf(FG_I_YELLOW);
    return;
  }
  else if (!strcmp(clr, "italic"))
  {
    printf(FG_ITALIC);
    return;
  }
  else if (!strcmp(clr, "ul"))
  {
    printf(FG_UL);
    return;
  }
  else if (!strncmp(clr, "lt", 2))
  {
    printf("\033[1m");
    clr += 2;
  }
  else
  {
    printf("\033[0m");
  }

  for (i = 0; i < 8; i++)
  {
    if (!strcmp(clr, colors[i]))
    {
      printf("\033[%dm", 30 + i);
      break;
    }
  }
  return;
} /* fgcolor */


void bgcolor(char *clr) {
  unsigned int i;

  if (
    !strcmp(clr, "reset")
     || !strcmp(clr, "clear")
    )
  {
    return(bgcolor("off"));
  }

  if (
    !strcmp(clr, "off")
    )
  {
    printf(BG_RESET);
    return;
  }

  for (i = 0; i < 8; i++)
  {
    if (!strcmp(clr, colors[i]))
    {
      printf("\033[%dm", 40 + i);
      break;
    }
  }
  return;
}


ssize_t cksys(const char *msg, ssize_t r) {
  if (r >= 0)
  {
    return(r);
  }
  fprintf(stderr, "Fatal Error in %s: %s\n", msg, strerror(errno));
  _exit(1);
}


void safe_ftruncate(int fd, off_t len) {
  while (true)
  {
    int r = ftruncate(fd, len);
    if (r == -1 && errno == EINTR)
    {
      continue;
    }
    cksys("ftruncate()", r);
    return;
  }
}


void color_usage(void){
  printf("%sc%so%sl%so%sr%s v%0.2f - copyright (c) 2001 "
         "Moshe Jacobson <moshe@runslinux.net>\n", FG_LTRED, FG_LTYELLOW,
         FG_LTGREEN, FG_LTBLUE, FG_LTMAGENTA, NOCOLOR, VERSION);
  printf("This program is free software released under the GPL. "
         "See COPYING for info.\n\n");
  printf("Usage:\n\n   %scolor%s [lt]%sfgcolor%s [%sbgcolor%s]\n",
         FG_BD, NOCOLOR, FG_UL, NOCOLOR, FG_UL, NOCOLOR);
  printf("   %scolor%s [ bd | ul | off ] %s\n   color%s --list\n\n",
         FG_BD, NOCOLOR, FG_BD, NOCOLOR);

  printf("   %sfgcolor%s and %sbgcolor%s are one of:\n",
         FG_UL, NOCOLOR, FG_UL, NOCOLOR);
  printf("      %sred %sgreen %syellow %sblue %smagenta %scyan %swhite%s.\n",
         FG_LTRED, FG_LTGREEN, FG_LTYELLOW, FG_LTBLUE, FG_LTMAGENTA,
         FG_LTCYAN, FG_LTWHITE, NOCOLOR);
  printf("   Preceed the foreground color with %slt%s to use a light "
         "color.\n", FG_BD, NOCOLOR);
  printf("   %scolor ul%s and %scolor bd%s turn on %sunderline%s and "
         "%sbold%s, respectively.\n", FG_BD, NOCOLOR, FG_BD, NOCOLOR,
         FG_UL, NOCOLOR, FG_BD, NOCOLOR);
  printf("   %scolor off%s turns off any coloring and resets "
         "to default colors.\n", FG_BD, NOCOLOR);
  printf("   %scolor --list%s shows all the possible color combinations "
         "visually.\n\n", FG_BD, NOCOLOR);

  printf("Example:\n\n");

  printf("   This program can be used from shellscripts as a "
         "convenience to allow for\n"
         "   ansi colored text output. Simply invoke it with command "
         "substitution. For\n"
         "   example, in a POSIX compliant shell such as bash or ksh, "
         "you would do:\n\n"
         "      echo \"$(color ltyellow blue)Hi there!$(color off)\"\n\n"
         "   to see %sHi there!%s\n\n", FG_LTYELLOW BG_BLUE, NOCOLOR);
  return;
}


int transfer_splice(int fdin, int fdout) {
  for (size_t cnt = 0; true; cnt++)
  {
    // Transferring 2GB at a time; this should be portable for 32bit
    // systems (and linux complains at the max val for uint64_t)
    ssize_t r = splice(fdin, NULL, fdout, NULL, ((size_t)1) << 31, 0);
    if (r == 0)
    {
      return(0);          // We're done
    }
    if (r < 0 && errno == EINTR)
    {
      continue;
    }
    if (r < 0 && errno == EINVAL && cnt == 0)
    {
      return(-1);                                        // File not supported
    }
    cksys("splice()", r);
  }
}


static SHELL_VAR *get_mypid(SHELL_VAR *var)                  {
  int  rv;
  char *p;

  rv = getpid();
  p  = itos(rv);

  FREE(value_cell(var));

  VSETATTR(var, att_integer);
  var_setvalue(var, p);
  return(var);
}


static SHELL_VAR *assign_mypid(SHELL_VAR  *self,
                               char       *value,
                               arrayind_t unused,
                               char       *key){
  return(self);
}


long millis(){
  struct timespec _t;

  clock_gettime(CLOCK_REALTIME, &_t);
  return(_t.tv_sec * 1000 + lround(_t.tv_nsec / 1e6));
}


uint64_t get_now_time() {
  struct timespec spec;

  if (clock_gettime(1, &spec) == -1)   /* 1 is CLOCK_MONOTONIC */
  {
    abort();
  }

  return(spec.tv_sec * 1000 + spec.tv_nsec / 1e6);
}


/*
 * static SHELL_VAR *
 * get_ms (SHELL_VAR *var)
 * {
 * char *p;
 *      struct timeval tv = { 0 };
 *      if (gettimeofday(&tv, NULL) < 0) {
 *              builtin_error("Failed to get time of day: %m");
 *              return NULL;
 *      }
 * long int TS = currentTimeMillis();
 *      if (asprintf(&p, "%lld", TS) < 0) {
 *              builtin_error("Failed to get memory for time of day: %m");
 *              return NULL;
 *      }
 *      VSETATTR(var, att_integer);
 *      var_setvalue(var, p);
 * return var;
 * }
 */
static SHELL_VAR *get_ts(SHELL_VAR *var)                  {
  char           *p;
  struct timeval tv = { 0 };

  if (gettimeofday(&tv, NULL) < 0)
  {
    builtin_error("Failed to get time of day: %m");
    return(NULL);
  }
  int TS = tv.tv_sec;

  if (asprintf(&p, "%d", TS) < 0)
  {
    builtin_error("Failed to get memory for time of day: %m");
    return(NULL);
  }
  VSETATTR(var, att_integer);
  var_setvalue(var, p);
  return(var);
}


static SHELL_VAR *assign_var(SHELL_VAR  *self,
                             char       *value,
                             arrayind_t unused,
                             char       *key){
  return(self);
}


int color_builtin_load(s) char *s;

{
  INIT_DYNAMIC_VAR("TS", (char *)NULL, get_ts, assign_var);
//    INIT_DYNAMIC_VAR ("MS", (char *)NULL, get_ms, assign_var);
  INIT_DYNAMIC_VAR("MYPID", (char *)NULL, get_mypid, assign_var);
  SHELL_VAR *v1 = find_variable("V1");
  if (v1 != NULL)
  {
    set_var_read_only("V1");
    printf("Environment Variable %s is set to %s and has been set to read only\n", "V1", get_variable_value(v1));
  }
  fflush(stdout);
  return(1);
}


void list_colors(){
  unsigned int bg, fg, bd;

  for (bg = 0; bg < 8; bg++)
  {
    for (bd = 0; bd <= 1; bd++)
    {
      printf("%s:\n", colors[bg]);
      for (fg = 0; fg < 8; fg++)
      {
        printf(" \e[%dm\e[%s%dm %s%s \e[0m",
               bg + 40, bd?"1;":"", fg + 30, bd?"lt":"  ", colors[fg]);
        fflush(stdout);
      }
      printf("\n");
    }
  }
  return;
}
int color_builtin(list) WORD_LIST *list;

{
  int qty         = 0;
  int on_fg_style = 0;
  int on_fg_color = 0;
  int on_bg_color = 0;
  for (int i = 1; list != NULL; list = list->next, ++i)
  {
    qty++;
    if (on_bg_color == 1)
    {
      bgcolor(list->word->word);
      on_bg_color = 0;
    }
    if (on_fg_color == 1)
    {
      fgcolor(list->word->word);
      on_fg_color = 0;
    }
    if ((strcasecmp(list->word->word, "--bgcolor") == 0) || (strcasecmp(list->word->word, "-b") == 0) || (strcasecmp(list->word->word, "b") || (strcasecmp(list->word->word, "bg") == 0) || ((strcasecmp(list->word->word, "--bcolor") == 0) == 0)))
    {
      on_bg_color = 1;
    }
    if ((strcasecmp(list->word->word, "--color") == 0) || (strcasecmp(list->word->word, "-c") == 0) || (strcasecmp(list->word->word, "color") == 0) || (strcasecmp(list->word->word, "fg") == 0))
    {
      on_fg_color = 1;
    }

    if ((strcasecmp(list->word->word, "--help") == 0) || (strcasecmp(list->word->word, "-h") == 0) || (strcasecmp(list->word->word, "usage") == 0))
    {
      color_usage();
      return(EXECUTION_SUCCESS);
    }

    if (strcasecmp(list->word->word, "--italic") == 0)
    {
      fgcolor("italic");
      return(EXECUTION_SUCCESS);
    }
    if (strcasecmp(list->word->word, "--bold") == 0)
    {
      fgcolor("bd");
      return(EXECUTION_SUCCESS);
    }
    if (strcasecmp(list->word->word, "--underline") == 0)
    {
      fgcolor("ul");
      return(EXECUTION_SUCCESS);
    }
    if (
      (strcasecmp(list->word->word, "--strike") == 0))
    {
      fgcolor("strike");
      return(EXECUTION_SUCCESS);
    }
    if (
      (strcasecmp(list->word->word, "--faint") == 0)
       || (strcasecmp(list->word->word, "--faint") == 0))
    {
      fgcolor("faint");
      return(EXECUTION_SUCCESS);
    }

    if (strcasecmp(list->word->word, "--list") == 0)
    {
      list_colors();
      return(EXECUTION_SUCCESS);
    }

    if (  (strcasecmp(list->word->word, "off") == 0) || (strcasecmp(list->word->word, "clear") == 0) || (strcasecmp(list->word->word, "reset") == 0) || (strcasecmp(list->word->word, "off") == 0)
       || (strcasecmp(list->word->word, "--reset") == 0)
          )
    {
      fgcolor("off");
      bgcolor("off");
      return(EXECUTION_SUCCESS);
    }
  }
  if (qty == 0)
  {
    color_usage();
    return(0);
  }

/*
 *  fgcolor("blue");
 *  bgcolor("black");
 *  printf("hello color");
 *  fgcolor("off");
 *  bgcolor("off");
 *  printf("\n");
 */
  fflush(stdout);
  return(EXECUTION_SUCCESS);
}


void color_builtin_unload(s) char *s;

{
  //  printf("color builtin unloaded\n");
  fflush(stdout);
}

char *color_doc[] =
{
  "Color builtin.",
  "",
  "this is the long doc for the sample color builtin",
  (char *)NULL
};

struct builtin color_struct =
{
  "color",                      /* builtin name */
  color_builtin,                /* function implementing the builtin */
  BUILTIN_ENABLED,              /* initial flags for builtin */
  color_doc,                    /* array of long documentation strings. */
  "color",                      /* usage synopsis; becomes short_doc */
  0                             /* reserved for internal use */
};
