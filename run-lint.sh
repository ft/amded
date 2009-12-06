#!/bin/sh

POSIX_SHELL=${POSIX_SHELL:-"/bin/sh"}
LINT=${LINT:-splint}

files=$(find . -name "*.c" -print)
fn=0
fe=0
filesn=0

skip() {
    file="$1"
    case "${file}" in
        # bsdgetopt.c is included from taggit.c and therefore doesn't
        # need to get checked on its own. In fact, that would turn up
        # a false positive.
        ./bsdgetopt.c|bsdgetopt.c) return 0;;
    esac
    return 1
}

for file in ${files}; do
    skip "${file}" && continue
    filesn=$(( ${filesn} + 1 ))
done

printf -- ' -- Running Lint (%s) in "%s" (%d files)\n' "${LINT}" "$(pwd)" "$filesn"
for file in ${files}; do
    skip "${file}" && continue
    fn=$(( $fn + 1 ))
    printf -- '\n -- Lint -- %04d/%04d -- %s --\n' $fn ${filesn} "${file}"
    ${LINT} "$@" "${file}"
    [ $? -ne 0 ] && fe=$(( $fe + 1 ))
done

printf '\nLint run finished.\n'
printf '%s files checked - %s had warnings (%s%%)\n\n' $fn $fe $(( $fe * 100 / fn ))
