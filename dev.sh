#!/bin/bash
set -e
DEFAULT_CMD="./test.sh"
cmd="${@:-$DEFAULT_CMD}"
flock_pfx="flock -x /var/spool/bash-loadables.lock"

make_cmd="make || { make clean && make; } && eval $cmd"

cmd="reap passh nodemon --signal SIGKILL -i src/\*.lo -i src/\*.la  -w \*.sh -w src -e c,sh,h --delay .2 -V -x /bin/sh -- -c '$flock_pfx sh -c \"$make_cmd\"||true'"

eval "$cmd"
