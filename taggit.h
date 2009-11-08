/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#include <taglib/tag_c.h>

#define PROJECT "taggit"
#define VERSION "0.0"

#define TAGGIT_USAGE \
"taggit OPTION(s) FILE(s)\n" \
"  informational options:\n" \
"    -h                display this help text\n" \
"    -L                show taggit's licence information\n" \
"    -v                print version information\n" \
"  action options:\n" \
"    -l                list tags in human readable form\n" \
"    -m                list tags in machine readable form\n" \
"    -t <tag>=<value>  set a tag to a value\n"

#define TAGGIT_LICENCE \
" Copyright 2009 taggit workers, All rights reserved.\n" \
"\n" \
" Redistribution and use in source and binary forms, with or without\n" \
" modification, are permitted provided that the following conditions\n" \
" are met:\n" \
"\n" \
" 1. Redistributions of source code must retain the above copyright\n" \
"    notice, this list of conditions and the following disclaimer.\n" \
" 2. Redistributions in binary form must reproduce the above copyright\n" \
"    notice, this list of conditions and the following disclaimer in the\n" \
"    documentation and/or other materials provided with the distribution.\n" \
"\n" \
" THIS SOFTWARE IS PROVIDED \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES,\n"\
" INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY\n" \
" AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL\n" \
" THE AUTHOR OR CONTRIBUTORS OF THE PROJECT BE LIABLE FOR ANY DIRECT,\n" \
" INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n" \
" (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n" \
" SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)\n" \
" HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,\n" \
" STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING\n" \
" IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n" \
" POSSIBILITY OF SUCH DAMAGE.\n"

struct taggit_list {
    char *artist;
    char *album;
    int tracknumber;
    char *tracktitle;
    int year;
    char *genre;
    int bitrate;
    int kbitrate;
    int samplerate;
    double ksamplerate;
    int channels;
    int length;
    int seconds;
    int minutes;
};

void taggit_list_human(const char *);
void taggit_list_machine(const char *);
void taggit_tag(const char *);

struct taggit_list *list(TagLib_File *);
