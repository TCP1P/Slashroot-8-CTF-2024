#!/bin/sh
set -eu
printf '%s\n' "$RSCTF_FLAG" > /srv/flag
chmod 0444 /srv/flag
