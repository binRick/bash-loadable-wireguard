#!/usr/bin/env bash
set -e
cd $(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
export PATH=$PATH:$(pwd)/bin
ARGS="${@:-}"
RCFILE=$(mktemp)
echo -e "enable -f libs/wg.so wg" > $RCFILE

test_cmd(){

v1=123
cmd="$(cat << EOF1
command env command bash --noprofile --norc -il << EOF
enable -f libs/wg.so wg
eval wg $ARGS
EOF
EOF1
)"

ansi --yellow --italic "$cmd"

while read -r l; do
  msg="<$(ansi --red --bg-black --bold "$$")> [$(ansi --blue --bold --bg-black BASH)] $(ansi --magenta --italic "$l")"
 >&2  echo -e "$msg"
#  echo -e "$l"
done < <(eval "$cmd")

}

do_shell(){
  cmd="command env command bash --rcfile $RCFILE -i"
 >&2  ansi --yellow --italic "$cmd"
  eval "$cmd"
  
}


if [[ "$ARGS" == "" ]]; then
  do_shell
else
  test_cmd
fi
