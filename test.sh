#!/usr/bin/env bash
set -e
cd $(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
export PATH=$PATH:$(pwd)/bin 

if ! command -v ansi >/dev/null; then
  alias ansi=$(pwd)/ansi
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

#test_builtin color color "$DEFAULT_POST_CMD"
#test_builtin color color "for x in \$(seq 1 5); do echo -e \"TS=\$TS|MS=\$MS\"; sleep 2; done"
WIREGUARD_LISTEN_PORT=2777 WIREGUARD_INTERFACE_NAME= test_builtin wg wg "wg"
WIREGUARD_LISTEN_PORT=1999 WIREGUARD_INTERFACE_NAME=wgtest10 test_builtin wg wg "wg"
WIREGUARD_LISTEN_PORT=1888 WIREGUARD_INTERFACE_NAME=wgtest11 test_builtin wg wg "wg"
WIREGUARD_LISTEN_PORT=     WIREGUARD_INTERFACE_NAME=wgtest12 test_builtin wg wg "wg"
WIREGUARD_LISTEN_PORT=2001 WIREGUARD_INTERFACE_NAME=wgtest13 test_builtin wg wg "wg"
WIREGUARD_PRIVATE_KEY="qJKSCynonUUyR8oOYD174ppCa77e0h1aC4Uh/QnHaXo=" WIREGUARD_LISTEN_PORT=2002 WIREGUARD_INTERFACE_NAME=wgtest14 test_builtin wg wg "wg"
echo OK
