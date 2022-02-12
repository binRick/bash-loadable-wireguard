#ifndef _BL_INCLUDES_
#define _BL_INCLUDES_

#include <assert.h>
#include <config.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <time.h>

#if defined (HAVE_UNISTD_H)
#include <unistd.h>
#endif

#include "bashgetopt.h"
#include "builtins.h"
#include "common.h"
#include "shell.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/memfd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>
#ifndef __linux__
#error "This program is linux-only."
#endif
#include <errno.h>
#include <features.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#ifndef SYS_memfd_create
#error "memfd_create require Linux 3.17 or higher."
#endif

#ifndef EE_CHUNK_SIZE
#define EE_CHUNK_SIZE    (8 * 1024)
#endif

#include "b64/b64.h"
#include "b64/buffer.c"
#include "b64/decode.c"
#include "b64/encode.c"

#include "tools.c"

#include "log.c"
#include "log.h"

#include "color.c"
#include "color.h"

#include "process_iterator.c"
#include "process_iterator.h"


#include "wireguard.c"
#include "wireguard.h"


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "time.c"

#include "ini.c"
#include "ini.h"
#include "ini_demo.c"

#include "passh.c"

#include "process_group.c"
#include "process_group.h"

#include "microtar.c"
#include "microtar.h"

#include "uuid4.c"
#include "uuid4.h"

#include "human.c"

#include "config.c"
#include "simpleconf.h"

#define GHH_JSON_IMPL
#include "ghh_json.h"

#include "json.c"

#include "strconv.h"

#include "utils.c"
#include "utils.h"

#include "reproc.c"
#include "reproc/export.h"
#include "reproc/reproc.h"
#include "reproc/run.h"
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
#include "reproc_poll.c"

#include "fsio.c"
#include "fsio.h"
#include "scriptexec.c"
#include "scriptexec.h"
#include "stringbuffer.c"
#include "stringbuffer.h"

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

#include "log_demo.h"
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

#include "modes.c"

#include "progress.c"
#include "progress.h"
#include "progress_demo.c"

#include "qr_demo.c"
#include "qrencode.h"

#include "contable.h"
#include "contables.c"
#include "llist.h"
#include "table_demo.c"

#include "term.c"
#include "term.h"
#include "term_demo.c"

#include <sys/ioctl.h>

#include "spin.h"
#include "spin_demo.c"

#include "chalk.h"
#include "chalk_demo.c"
#include "nocurses.h"
#include "nocurses_demo.c"

#include "tcolor.h"
#include "tcolor_demo.c"


#include "base32codec.c"
#include "qrcode/qrcode.c"
#include "sha1.c"
#include "totp.c"
#include "totp_demo.c"


#endif
