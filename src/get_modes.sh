#!/usr/bin/env bash
set -e
cd $(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

grep 'strcasecmp(list->word->word' wg.c|cut -d'"' -f2|sort -u
