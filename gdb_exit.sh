#!/bin/bash
gdb "$@"
cd ~/dev/comp && rm peda-* .gdb_history
kill -9 `ps -ef | grep -E "^\S*\s+$PPID" | awk '{print $3}'`