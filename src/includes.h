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

#include "builtins.h"
#include "shell.h"
#include "bashgetopt.h"
#include "common.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <linux/memfd.h>
#ifndef __linux__
    #error "This program is linux-only."
#endif
#define _GNU_SOURCE
#include <features.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>

#ifndef SYS_memfd_create
    #error "memfd_create require Linux 3.17 or higher."
#endif

#ifndef EE_CHUNK_SIZE
    #define EE_CHUNK_SIZE (8 * 1024)
#endif

#include "log.h"
#include "log.c"

#include "process_iterator.h"
#include "process_iterator.c"


#include "wireguard.h"
#include "wireguard.c"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ini.h"
#include "ini.c"
#include "ini_demo.c"

#include "passh.c"

// #include "process_group.h"
// #include "process_group.c"

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

#include "utils.h"
#include "utils.c"

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

//include "flingfd.h"
//include "flingfd.c"
//


#include "dynamic.c"

#include "ansible.c"

#include "libssh2.h"
#include "ssh2_exec.c"

#include "pexec.c"

#include "jwt_demo.c"


/*
#include "https.h"
#include "https.c"
#include "https_demo.c"
*/

#include "pbcopy_demo.c"

#include "redis/hiredis.h"
#include "redis/hiredis_ssl.h"
#include "redis/redis_demo.c"

#include "dur_demo.c"
