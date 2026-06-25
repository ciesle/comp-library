#!/bin/bash

append_sanitizer_options() {
    local var_name="$1"
    local options="$2"
    local current="${!var_name:-}"

    if [[ -n "$current" ]]; then
        printf -v "$var_name" '%s:%s' "$current" "$options"
    else
        printf -v "$var_name" '%s' "$options"
    fi
    export "$var_name"
}

append_sanitizer_options ASAN_OPTIONS "abort_on_error=1:halt_on_error=1:symbolize=1"
append_sanitizer_options UBSAN_OPTIONS "abort_on_error=1:halt_on_error=1:print_stacktrace=1:symbolize=1"

gdb \
    -ex "set breakpoint pending on" \
    -ex "handle SIGABRT stop print pass" \
    -ex "rbreak __asan_report_.*" \
    -ex "rbreak __ubsan_handle_.*" \
    "$@"

cd ~/dev/comp && rm -f peda-* .gdb_history
kill -9 `ps -ef | grep -E "^\S*\s+$PPID" | awk '{print $3}'`
