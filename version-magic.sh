#!/bin/sh

getgit_ () {
    base_="$(git describe --abbrev=12)"
    dirty_=""
    git update-index -q --refresh
    [ -z "$(git diff-index --name-only HEAD --)" ] || dirty_="-dirty"
    printf '%s%s' "${base_}" "${dirty_}"
}
getdesc_ () {
    git show -s --pretty="tformat:%h (%s, %ai" | \
    sed -e "s~ [012][0-9]:[0-5][0-9]:[0-5][0-9] [-+][0-9][0-9][0-9][0-9]$~)~"
}

if ! git rev-parse --is-inside-work-tree > /dev/null 2>&1 ; then
    make_macro='PLAIN_SOURCE'
    make_deps='info.cpp'
else
    make_macro='GIT_SOURCE'
    make_deps='info.cpp git-version.h'
    gitdesc="$(getdesc_)"
    if [ -e git-version.h ] ; then
        while IFS= read -r line; do
            case "${line}" in
                '#define GIT_VERSION'*)
                    gitversion_old="${line#\#define*VERSION}"
                    gitversion_old="${gitversion_old##* \"}"
                    gitversion_old="${gitversion_old%\"}"
                    break
                    ;;
                *) ;;
            esac
        done < ./git-version.h

        gitversion_new="$(getgit_)"
        if [ "${gitversion_old}" != "${gitversion_new}" ] ; then
            printf 'version-magic.sh: Updating git version: %s\n' \
                "${gitversion_new}"
            sed -e 's~@@GIT-VERSION@@~'"${gitversion_new}"'~g' \
                -e 's~@@GIT-DESCRIPTION@@~'"${gitdesc}"'~g' \
                < ./git-version.h.in > git-version.h
        fi
    else
        gitversion="$(getgit_)"
        printf 'version-magic.sh: Setting up git version: %s\n' \
            "${gitversion}"
        sed -e 's~@@GIT-VERSION@@~'"${gitversion}"'~g' \
            -e 's~@@GIT-DESCRIPTION@@~'"${gitdesc}"'~g' \
            < ./git-version.h.in > git-version.h
    fi
fi

if [ ! -e version-magic.make ]; then
    sed -e 's~@@MAKE-MACRO@@~'"${make_macro}"'~g' \
        -e 's~@@MAKE-DEPS@@~'"${make_deps}"'~g' \
        < ./version-magic.make.in > version-magic.make
fi
exit 0
