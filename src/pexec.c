extern char **environ;
struct argv_struct { char **new_argv; int new_argc; } NEW_ARGV[0];

int exit_failure(const char* msg){
    perror(msg);
    return EXIT_FAILURE;
}
char *binaries[100];

char **get_binaries(){
  return binaries;
}

int pexec_main(int argc, char **argv) {
  int     memfd;
  ssize_t nread;
  ssize_t nwrite;
  size_t  offset;
  size_t  length;
  char    buf[EE_CHUNK_SIZE];
  struct timespec pexec_start, pexec_end;

  clock_gettime(CLOCK_MONOTONIC_RAW, &pexec_start);
  memfd = (int)syscall(SYS_memfd_create, argv[0], 0);
  if (memfd == -1){
    log_error("Failed to create memfd");
    return exit_failure("memfd_create");
  }
  int total = 0;
  do {
        nread = read(STDIN_FILENO, buf, EE_CHUNK_SIZE);
        if (nread == -1)
            return exit_failure("read");
        //log_debug("Read %d Bytes from stdin", nread);
        offset = 0;
        length = (size_t)nread;
        while (offset < length) {
            nwrite = write(memfd, buf + offset, length - offset);
            if (nwrite == -1)
                return exit_failure("write");
            //log_debug("Wrote %d Bytes to memfd", nwrite);
            total = total + nwrite;
            offset += (size_t)nwrite;
        }
  } while (nread > 0);
  clock_gettime(CLOCK_MONOTONIC_RAW, &pexec_end);
  if(pexec_end.tv_nsec < pexec_start.tv_nsec){
    pexec_end.tv_nsec += 1000000000;
    pexec_end.tv_sec--;
  }
  log_debug("Processed %d Byte Binary in %ld.%09lds",total, (long)(pexec_end.tv_sec - pexec_start.tv_sec), (pexec_end.tv_nsec - pexec_start.tv_nsec));

  if (fexecve(memfd, argv, environ) == -1)
    return exit_failure("fexecve");

  return exit_failure("fexecve failed to execute");
}
int pexec_demo(int argc, char **argv){
  log_debug("pexec_demo.........%d args", argc);
  char *new_argv[argc];
  int new_argc = create_submode_argc_argv(new_argv, argc, argv);
  log_debug("pexec_demo>create_submode_argc_argv: new argc: %d", new_argc);
  for (int i = 0; i < new_argc; i++) {
    log_debug(">>>>>>>>>>>> pexec new argv #%d => %s", i, new_argv[i]);
  }
  int res = pexec_main(new_argc, new_argv);
  log_debug("pexec main returned: %d", res);
  return res;
}

