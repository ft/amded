/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "taggit.h"
#include "bsdgetopt.c"

enum t_mode {
    TAGGIT_MODE_INVALID = 0,
    TAGGIT_LIST_HUMAN,
    TAGGIT_LIST_MACHINE,
    TAGGIT_TAG
};

enum t_mode taggit_mode = TAGGIT_MODE_INVALID;

void
check_mode(enum t_mode mode)
{
    if (mode != TAGGIT_MODE_INVALID) {
        printf("-m, -l and -t may *not* be used at the same time.\n");
        exit(EXIT_FAILURE);
    }
}

void
parse_options(int argc, const char *argv[])
{
    int err, opt;

    err = 1;
    while ((opt = bsd_getopt(argc, argv, "hLlmt:v")) != -1) {
        switch (opt) {
        case 'L':
            printf(TAGGIT_LICENCE);
            err = 0;
            goto quit;
        case 'l':
            check_mode(taggit_mode);
            taggit_mode = TAGGIT_LIST_HUMAN;
            break;
        case 'm':
            check_mode(taggit_mode);
            taggit_mode = TAGGIT_LIST_MACHINE;
            break;
        case 't':
            check_mode(taggit_mode);
            taggit_mode = TAGGIT_TAG;
            break;

        case 'v':
            printf("%s v%s\n", PROJECT, VERSION);
            err = 0;
            goto quit;
        case 'h':
            err = 0;
            /*@FALLTHROUGH@*/
        default:
            printf(TAGGIT_USAGE);
            goto quit;
        }
    }

    return;

quit:
    if (!err)
        exit(EXIT_SUCCESS);
    else
        exit(EXIT_FAILURE);
}

int
main(int argc, const char *argv[])
{
    int i;

    if (argc < 2) {
        printf(TAGGIT_USAGE);
        return EXIT_FAILURE;
    }

    parse_options(argc, argv);

    if (optind == argc) {
        printf(TAGGIT_USAGE);
        return EXIT_FAILURE;
    }

    for (i = optind; i < argc; ++i) {
        switch (taggit_mode) {
        case TAGGIT_LIST_HUMAN:
            taggit_list_human(argv[i]);
            break;
        case TAGGIT_LIST_MACHINE:
            taggit_list_machine(argv[i]);
            break;
        case TAGGIT_TAG:
            taggit_tag(argv[i]);
            break;
        default:
            printf("Please use one action option (-m, -l or -t).\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
