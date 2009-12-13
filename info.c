/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  info.c
 * @brief display various informational messages
 */

#include <stdio.h>

#include "taggit.h"

#ifdef GIT_SOURCE
#include "git-version.h"
#endif /* GIT_SOURCE */

/** usage information */
static const char *usage[] = {
"usage: taggit OPTION(s) FILE(s)",
"",
"  informational options:",
"    -h                display this help text",
"    -L                show taggit's licence information",
"    -v                print version information",
"    -s                list supported tag names",
"  configuration options:",
"    -R <readmap>      configure tag reading order",
"    -W <writemap>     configure which tag types should be written",
"  action options:",
"    -l                list tags in human readable form",
"    -m                list tags in machine readable form",
"    -t <tag>=<value>  set a tag to a value",
"",
(const char*)NULL
};

/** licence information */
static const char *licence[] = {
" Copyright 2009 taggit workers, All rights reserved.",
"",
" Redistribution and use in source and binary forms, with or without",
" modification, are permitted provided that the following conditions",
" are met:",
"",
" 1. Redistributions of source code must retain the above copyright",
"    notice, this list of conditions and the following disclaimer.",
" 2. Redistributions in binary form must reproduce the above copyright",
"    notice, this list of conditions and the following disclaimer in the",
"    documentation and/or other materials provided with the distribution.",
"",
" THIS SOFTWARE IS PROVIDED \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES,",
" INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY",
" AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL",
" THE AUTHOR OR CONTRIBUTORS OF THE PROJECT BE LIABLE FOR ANY DIRECT,",
" INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES",
" (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR",
" SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)",
" HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,",
" STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING",
" IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE",
" POSSIBILITY OF SUCH DAMAGE.",
(const char*)NULL
};

/**
 * Print an array of strings to stdout
 *
 * Appends a newline character to each string, rendering the output
 * line by line. The end of the array *must* be marked by a
 * (const char *)NULL entry.
 *
 * @param   a       pointer to a the array of c-strings
 *
 * @return      void
 * @sideeffects none
 */
static void
print_array(const char *a[])
{
    unsigned int i;

    for (i = 0; a[i] != NULL; ++i)
        printf("%s\n", a[i]);
}

/** Print version information about taggit */
void
taggit_version(void)
{
    printf("%s ", PROJECT);
#ifdef GIT_SOURCE
    printf("%s\n", GIT_VERSION);
    printf("Based on commit: %s\n", GIT_DESCRIPTION);
#else
    printf("%s\n", VERSION);
#endif
}

/** Print out usage information (contained in the usage array above) */
void
taggit_usage(void)
{
    print_array(usage);
}

/** Print out licencing information (contained in the licence array above) */
void
taggit_licence(void)
{
    print_array(licence);
}
