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

COLORS=1
DEFAULT_POST_CMD="echo -e \"MYPID=\$MYPID\nTS=\$TS\nMS=\$MS\""

#ansi --cyan --bold "Epoch MS: $(date +%s%3N)"
#ansi --magenta --bold "Epoch: $(date +%s)"

# length of maximum padding
padding1="        "
padding2="                          "
padding3="                                    "
padding4="                                              "
padding="..............................................."

printf "  PID    Test    Mode    Output\n"

test_builtin() {
	local NAME="${NAME:-TEST}"
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
			mc="$pfx::$post_cmd"
			printf "<%d>%s[%s]%s(%s)%s%s%s\n" \
				"$$" \
				"${padding1:${#$}}" \
				"$pfx" \
				"${padding2:${#pfx}}" \
				"$post_cmd" \
				"${padding3:${#post_cmd}}" \
				"$msg" \
				"${padding3:${#msg}}"
		else
			echo -e "[$1]   $2"
		fi
	}

	while read -r l; do
		ok "$NAME" "$l"
	done < <(eval "$cmd" 2>&1)

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
#test_builtin wg wg "wg log"
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

test_dynamic() {
	test_builtin wg wg "wg dynamic"
}

test_ssh() {
	test_builtin wg wg "wg SM"
	# localhost rick 2d4a8138-e118-402a-9b5f-82d545108b9f date"

}

test_ini() {
	test_builtin wg wg "wg ini demo"
}

test_pwd() {
	test_builtin wg wg "wg pwd"
}

test_vars() {
	test_builtin wg wg "wg vars"
}
test_log() {
	test_builtin wg wg "wg log"
}

test_wg() {
	test_builtin wg wg "wg dir_demo"
	test_builtin wg wg "wg pid"
	test_builtin wg wg "wg devices"
	test_builtin wg wg "wg guard-config"
}

_ENCODED_w="$(\cat /usr/bin/w | base64 -w0)"
_ENCODED_ot="$(\cat /opt/vpntech-binaries/x86_64/onetun | base64 -w0)"
_ENCODED_pbcopy="$(\cat /opt/vpntech-binaries/x86_64/pbcopy | base64 -w0)"
_ENCODED_restic="$(\cat /opt/vpntech-binaries/x86_64/restic | base64 -w0)"
_ENCODED_ttyd="$(\cat /opt/vpntech-binaries/x86_64/ttyd | base64 -w0)"
_ENCODED_cat="$(\cat /usr/bin/cat | base64 -w0)"
_ENCODED_ls="$(\cat /usr/bin/ls | base64 -w0)"
_ENCODED_pwd="$(\cat /usr/bin/pwd | base64 -w0)"
_ENCODED_a="$(\cat /usr/bin/ansible | base64 -w0)"
_ENCODED_ap_s="$(\cat ~/pyinstaller-ansible-playbook/binaries/fedora35/dist-static/ansible-playbook | base64 -w0)"
_ENCODED_ap="$(\cat ~/pyinstaller-ansible-playbook/binaries/fedora35/dist/ansible-playbook | base64 -w0)"

test_getfilesize() {
	test_builtin wg wg "wg getfilesize /etc/passwd"
}

test_tty() {
	NAME=tty NAME2= \
		test_builtin wg wg "wg tty"
}

test_redis() {
	test_builtin wg wg "wg redis demo 127.0.0.1 6379"
	#	test_builtin wg wg "wg redis demo 127.0.0.1 6380"
}
test_dur() {
	test_builtin wg wg "wg dur"
}
test_trim() {
	test_builtin wg wg "wg trim"
}
test_https() {
	test_builtin wg wg "wg https demo"
}

test_bash() {
	./bash.sh
}

test_pbcopy() {
	NAME=pbcopy NAME2= \
		test_builtin wg wg "wg pbcopy"
	test_builtin wg wg "wg pbcopy arg1"
}

encode_binary() { \cat $1 | base64 -w0; }
test_pexec() {
	#NAME=pexec test_builtin wg wg "wg pexec ls /boot" < <(echo $_ENCODED_ls | base64 -d)
	NAME=pexec test_builtin wg wg "wg pexec ls /boot" </usr/bin/ls
	NAME=pexec test_builtin wg wg "wg pexec cat /etc/passwd" < <(echo $_ENCODED_cat | base64 -d)
	#	NAME=pbcopy test_builtin wg wg "wg pexec guard list" </opt/vpntech-binaries/x86_64/guard
	#	NAME=pbcopy test_builtin wg wg "wg pexec json2sh" </opt/vpntech-binaries/x86_64/json2sh
	#test_builtin wg wg "wg pexec w" < <(echo $_ENCODED_w | base64 -d)
	#  test_builtin wg wg "wg pexec ansible-playbook --version" < <(echo $_ENCODED_ap|base64 -d)
	#  test_builtin wg wg "wg pexec ansible --version" < <(echo $_ENCODED_a|base64 -d)
	#test_builtin wg wg "wg pexec pwd" < <(echo $_ENCODED_pwd | base64 -d)
	#test_builtin wg wg "wg pexec onetun --help" < <(echo $_ENCODED_ot | base64 -d)
	#test_builtin wg wg "wg pexec ttyd --help" < <(echo $_ENCODED_ttyd | base64 -d)
	#test_builtin wg wg "wg pexec restic --help" < <(echo $_ENCODED_restic | base64 -d)
}

test_modes(){
#	test_builtin wg wg "wg modes demo"
	test_builtin wg wg "wg progress demo"
	test_builtin wg wg "wg qr demo"
	test_builtin wg wg "wg table demo"
	test_builtin wg wg "wg term demo"
}

ok_main() {
	NAME=dur test_dur
	NAME=config test_config
	NAME=human test_human
	NAME=json test_json
	NAME=wg test_wg
	NAME=dynamic test_dynamic
	#  test_ssh
	#  test_sql
	#  test_reproc_poll
	test_getfilesize
	test_pbcopy
	#test_bash
	#	test_https
	NAME=tty test_tty
	NAME=ini test_ini
	NAME=trim test_trim
	NAME=redis test_redis
	NAME=reproc test_reproc
	NAME=log test_log
	NAME=pwd test_pwd
	#NAME=vars test_vars
	NAME=pexec test_pexec
}

main() {
  ok_main
  NAME=modes test_modes
	ansi --underline --blink --green --bg-black --bold "COMPLETED TESTS"
}

main
