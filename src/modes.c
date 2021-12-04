
char *modes[10000];

void *get_modes(){
  char          line[SC_MAX_ARG_LENGTH];
  char *file_name = "src/.modes.txt";
  unsigned int  line_count = 0;
  FILE         *fp = NULL;
  if ((fp = fopen(file_name, "r")) == NULL) {
    log_error("Unable to open [%s]: %s\n", file_name, strerror(errno));
    return NULL;
  }
  while (fgets(line, (int)(sizeof line), fp) != NULL) {
    chomp(line);
    line_count++;
    log_info("mode: %s", line);
  }
  return NULL;
}

int print_modes(){
  char *modes = get_modes();
  printf("%s\n", modes);
  return 0;
}

int modes_demo(){
/*
  list_colors();
  char *modes[10000];
  char          line[SC_MAX_ARG_LENGTH];
  char *file_name = "src/.modes.txt";
  unsigned int  line_count = 0;
  FILE         *fp = NULL;
  if ((fp = fopen(file_name, "r")) == NULL) {
    log_error("Unable to open [%s]: %s\n", file_name, strerror(errno));
    return -1;
  }
  while (fgets(line, (int)(sizeof line), fp) != NULL) {
    chomp(line);
    line_count++;
    log_debug("%s> #%d: %s", file_name, line_count, line);
    sprintf(modes,"%s %s",modes, line);
  }
*/
  log_info("modes: ");
  get_modes();
  return 0;
}
