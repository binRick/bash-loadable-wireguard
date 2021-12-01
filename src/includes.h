#include <config.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <sys/utsname.h>
#include <assert.h>
#include <stdint.h>

#if defined (HAVE_UNISTD_H)
#  include <unistd.h>
#endif

#include "process_iterator.h"
#include "process_iterator.c"

#include "builtins.h"
#include "shell.h"
#include "bashgetopt.h"
#include "common.h"

#include "wireguard.h"
#include "wireguard.c"

#include "utils.h"
#include "utils.c"

#include "log.h"
#include "log.c"

#include "ini.h"
#include "ini.c"

#include "passh.c"

#include "process_group.h"
#include "process_group.c"

#include "microtar.h"
#include "microtar.c"

#include "uuid4.h"
#include "uuid4.c"

#include "human.c"

#include "simpleconf.h"
#include "config.c"

#define GHH_JSON_IMPL
#include "ghh_json.h"

#include "json.c"

#include "strconv.h"

#include "reproc/run.h"
#include "reproc/reproc.h"
#include "reproc/export.h"
#include "reproc/src/clock.posix.c"
#include "reproc/src/drain.c"
#include "reproc/src/error.posix.c"
#include "reproc/src/handle.posix.c"
#include "reproc/src/init.posix.c"
#include "reproc/src/options.c"
#include "reproc/src/pipe.posix.c"
#include "reproc/src/process.posix.c"
#include "reproc/src/redirect.c"
#include "reproc/src/redirect.posix.c"
#include "reproc/src/reproc.c"
#include "reproc/src/run.c"
#include "reproc/src/strv.c"
#include "reproc/src/utf.posix.c"

#include "reproc.c"
#include "reproc_poll.c"
/* include "sql.c" */
