#!/usr/bin/env bash
set -e
cd $(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
export PATH=$PATH:$(pwd)/bin

BASH_TEST_PREFIX="command env command bash --noprofile"

if [[ "$1" == shell ]]; then
	rc=$(mktemp)
	cat <<EOF >$rc
BUILTIN_MODULES="\$(find src/.libs/ -type f -name "*.so")"
echo -e "\$BUILTIN_MODULES"|while read -r m; do 
  name="\$(basename \$m .so)"
  cmd="{ enable -f \$m \$name && enable -d \$name; }; enable -f \$m \$name"
  echo "\$cmd"
  eval "\$cmd"
done
ansi --cyan --bg-black "\$BUILTIN_MODULES"
ansi --blue --bold "\$LOAD_CMDS"
EOF
	rc_dat="$(ansi --yellow --italic "$(cat $rc)")"
	echo -e "Starting bash with rc file contents:\n$rc_dat"
	cmd="$BASH_TEST_PREFIX --rcfile $rc -i"
	eval "$cmd"
	unlink $rc
	exit 0
fi

COLORS=0
DEFAULT_POST_CMD="echo -e \"MYPID=\$MYPID\nTS=\$TS\nMS=\$MS\""

ansi --cyan --bold "Epoch MS: $(date +%s%3N)"
ansi --magenta --bold "Epoch: $(date +%s)"

test_builtin() {
	local M="$1"
	local N="$2"
	local post_cmd="${3:-$DEFAULT_POST_CMD}"
	local cmd="enable -f 'src/.libs/$M.so' $N && $N && $post_cmd"
	cmd="command env command bash --norc --noprofile -c '$cmd'"

	err() {
		pfx="$(ansi --red --bold "$1")"
		msg="$(ansi --yellow --italic "$2")"
		if [[ "$COLORS" == 1 ]]; then
			echo -e "[$pfx]   $msg"
		else
			echo -e "[$1]   $2"
		fi
	}

	ok() {
		pfx="$(ansi --green --bold "$1")"
		msg="$(ansi --yellow --italic "$2")"
		if [[ "$COLORS" == 1 ]]; then
			echo -e "[$pfx]   $msg"
		else
			echo -e "[$1]   $2"
		fi
	}

	while read -r l; do
		ok "TEST" "$l"
	done < <(eval "$cmd")

}
ANSIBLE_TEST="env ansible localhost -i localhost, -c local -m setup -a gather_subset=min"
ANSIBLE_TEST="env ANSIBLE_NO_TARGET_SYSLOG=True ANSIBLE_PYTHON_INTERPRETER=auto_silent ansible localhost -i localhost, -c local -m setup -a filter=ansible_virtualization_type --one-line"
ANSIBLE_TEST="env ANSIBLE_NO_TARGET_SYSLOG=True ANSIBLE_PYTHON_INTERPRETER=auto_silent ansible localhost -i localhost, -c local -m setup -a gather_subset=min --one-line"
test_reproc() {
#	test_builtin wg wg "wg reproc env"
	test_builtin wg wg "wg reproc $ANSIBLE_TEST; echo -e ANSIBLE_VIRTUALIZATION_TYPE=\$ANSIBLE_VIRTUALIZATION_TYPE; env|grep ^ANSIBLE_"
	test_builtin wg wg "wg schedule $ANSIBLE_TEST; echo -e ANSIBLE_VIRTUALIZATION_TYPE=\$ANSIBLE_VIRTUALIZATION_TYPE; env|grep ^ANSIBLE_"
#	test_builtin wg wg "wg reproc ls /"
}
test_reproc_poll() {
	test_builtin wg wg "wg poll"
}

test_sql() {
	test_builtin wg wg "wg sql"
}

test_json() {
	test_builtin wg wg "wg json"
}

test_human() {
	test_builtin wg wg "wg human 600"
	test_builtin wg wg "wg human -600"
	test_builtin wg wg "wg human -s 600"
	test_builtin wg wg "wg human -s -600"
}

test_config() {
  test_builtin wg wg "wg config"
}

#test_builtin color color "$DEFAULT_POST_CMD"
#test_builtin color color "for x in \$(seq 1 5); do echo -e \"TS=\$TS|MS=\$MS\"; sleep 2; done"
#WIREGUARD_LISTEN_PORT=2777 WIREGUARD_INTERFACE_NAME= test_builtin wg wg "wg"
#WIREGUARD_LISTEN_PORT=1999 WIREGUARD_INTERFACE_NAME=wgtest10 test_builtin wg wg "wg"
#WIREGUARD_LISTEN_PORT=1888 WIREGUARD_INTERFACE_NAME=wgtest11 test_builtin wg wg "wg"
#WIREGUARD_LISTEN_PORT=     WIREGUARD_INTERFACE_NAME=wgtest12 test_builtin wg wg "wg"
export WIREGUARD_LISTEN_PORT=2001 WIREGUARD_INTERFACE_NAME=wgtest13
#test_builtin wg wg "wg pid"
#test_builtin wg wg "wg ls"
#test_builtin wg wg "wg down"
#test_builtin wg wg "wg up"
#test_builtin wg wg "wg ls"
#test_builtin wg wg "wg pwd"
#test_builtin wg wg "wg log"
#test_builtin wg wg "wg ini"
#test_builtin wg wg "wg passh"
#test_builtin wg wg "wg passh -P Password -p mypassword ls /"
#test_builtin wg wg "wg passh -P password -p 1e80e41c-78a6-4870-92d4-fddd829ed8c9 passwd T"
#test_builtin wg wg "wg passh -P password -p 1e80e41c-78a6-4870-92d4-fddd829ed8c9 ./wrap.sh 1 passwd T"
#export __PASS=1e80e41c-78a6-4870-92d4-fddd829ed8c9
#test_builtin wg wg "wg passh -P 'password:' -p env:__PASS -c 20 -C -i -y -T -t 5 -- ./wrap.sh"
#test_builtin wg tar "wg tar 123"
#WIREGUARD_LISTEN_PORT=2001 WIREGUARD_INTERFACE_NAME=wgtest13 test_builtin wg wg "wg up"
#WIREGUARD_LISTEN_PORT=2001 WIREGUARD_INTERFACE_NAME=wgtest13 test_builtin wg wg "wg ls"
#WIREGUARD_PRIVATE_KEY="qJKSCynonUUyR8oOYD174ppCa77e0h1aC4Uh/QnHaXo=" WIREGUARD_LISTEN_PORT=2002
#export WIREGUARD_INTERFACE_NAME=wgtest99
#test_builtin wg wg "wg"
#test_builtin wg wg "ls"
#test_builtin wg wg "up"
#test_builtin wg wg "down"
main(){
#  test_config
#  test_human
#  test_json
#  test_sql
  test_reproc
  test_reproc_poll
  echo OK
}

main


