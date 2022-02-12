#define PRINT_PREFIX    "🍦"

#define MAX_ROW         64
#define MAX_COLUMN      256
#define LOG_VERSION     "0.1.0"

#ifndef LOG_H
#define LOG_H

static const char *_format_colors[] = {
  "",
  "\x1b[32m\"%s\"\x1b[0m",
  "\x1b[34m%d\x1b[0m",
  "\x1b[34m%ld\x1b[0m",
  "\x1b[34m%#x\x1b[0m",
  "\x1b[34m%.2f\x1b[0m",
  "\x1b[34m%.4lf\x1b[0m",
  "\x1b[34m%p\x1b[0m"
};

static const char *_level_colors[] = { "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m" };


typedef struct
{
  va_list    ap;
  const char *fmt;
  const char *file;
  struct tm  *time;
  void       *udata;
  int        line;
  int        level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

enum
{
  LOG_TRACE,
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL
};

#define log_trace(...)    log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...)    log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)     log_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)     log_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...)    log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...)    log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

const char * log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_quiet(bool enable);
int log_add_callback(log_LogFn fn, void *udata, int level);
int log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, int line, const char *fmt, ...);

#endif
