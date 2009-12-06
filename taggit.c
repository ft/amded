/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  taggit.c
 * @brief main() and high level option handling
 */

#include <stdio.h>
#include <stdlib.h>

#include "taggit.h"
#include "bsdgetopt.c"

/**
 * ASCII End-Of-Transmission character code. Used to seperate output for
 * different files in machine readable mode.
 */
#define ASCII_EOT (char)0x04

/**
 * Head of the linked list that holds the information gathered
 * from all (one or more) -t tag=value options.
 */
struct taglist *tags_head = NULL;

#if 0
#define TAGGIT_DEBUG
#endif

/** possible operation modes */
enum t_mode {
    /** no mode defined, yet; or broken operation mode */
    TAGGIT_MODE_INVALID = 0,
    /** list file's tags in human readable form */
    TAGGIT_LIST_HUMAN,
    /** list file's tags in machine readable form */
    TAGGIT_LIST_MACHINE,
    /** modify meta information in file(s) */
    TAGGIT_TAG
};

/** global variable describing taggit's operation mode */
static enum t_mode taggit_mode = TAGGIT_MODE_INVALID;

/**
 * Check that -m, -l and -t are not used with one another
 *
 * @param   mode    the enum t_mode value to check
 *
 * @return      void
 * @sideeffects Exists with EXIT_FAILURE on failure.
 */
static inline void
check_mode(enum t_mode mode)
{
    if (mode != TAGGIT_MODE_INVALID) {
        printf("-m, -l and -t may *not* be used at the same time.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Mode sanity check for tagging mode
 *
 * -t may be used multiple times, so run check_mode() only if not in
 * TAGGIT_TAG mode already.
 *
 * @param   mode    the enum t_mode value to check
 *
 * @return      void
 * @sideeffects see check_mode()
 */
static inline void
check_mode_tag(enum t_mode mode)
{
    if (mode != TAGGIT_MODE_INVALID && mode != TAGGIT_TAG)
        check_mode(mode);
}

/**
 * Parsing command line options
 *
 * Uses NetBSD's getopt() defined in bsdgetopt.c, which is a
 * traditional getopt implementation that supports short options
 * and doesn't do any permutations.
 *
 * This function should not perform any actions itself, but set
 * taggit_mode accordingly and let main()'s main loop handle everything.
 *
 * @param   argc    number of entries in *argv[]
 * @param   argv[]  list of arguments at startup.
 *
 * @return      void
 * @sideeffects Exists using EXIT_SUCCESS or EXIT_FAILURE when appropriate.
 */
static void
parse_options(int argc, const char *argv[])
{
    int opt;
    struct t_tag tag;

    while ((opt = bsd_getopt(argc, argv, "hLlmsR:t:v")) != -1) {
        switch (opt) {
        case 'L':
            taggit_licence();
            exit(EXIT_SUCCESS);
        case 'l':
            check_mode(taggit_mode);
            taggit_mode = TAGGIT_LIST_HUMAN;
            break;
        case 'm':
            check_mode(taggit_mode);
            taggit_mode = TAGGIT_LIST_MACHINE;
            break;
        case 's':
            printf("Supported tags:\n");
            list_tags();
            exit(EXIT_SUCCESS);
        case 'R':
            setup_readmap(optarg);
            break;
        case 't':
            check_mode_tag(taggit_mode);
            taggit_mode = TAGGIT_TAG;
            tag = next_tag(optarg);
            if (tag.type == TAG_INVALID) {
                fprintf(stderr, "Invalid tag name: \"%s\"\n", tag.name);
                exit(EXIT_FAILURE);
            }
            add_tag(&tag);
#ifdef TAGGIT_DEBUG
            printf("[%s] {%s} <%d>\n", tag.name, tag.value, tag.type);
#endif
            break;

        case 'v':
            taggit_version();
            exit(EXIT_SUCCESS);
        case 'h':
            taggit_usage();
            exit(EXIT_SUCCESS);
        default:
            taggit_usage();
            exit(EXIT_FAILURE);
        }
    }

    return;
}

/**
 * taggit: command line utility for listing and modifying meta
 *         information in audio files
 *
 * Interfacing KDE's taglib:
 *   <http://developer.kde.org/~wheeler/taglib.html>
 *
 * @param   argc    number of entries in *argv[]
 * @param   argv[]  list of arguments at startup.
 *
 * @return      EXIT_SUCCESS on normal execution;
 *              EXIT_FAILURE upon failure.
 * @sideeffects none
 */
int
main(int argc, const char *argv[])
{
    int i;
    boolean first;

    if (argc < 2) {
        taggit_usage();
        return EXIT_FAILURE;
    }

    parse_options(argc, argv);

    if (optind == argc) {
        taggit_usage();
        return EXIT_FAILURE;
    }

#ifdef TAGGIT_DEBUG
    if (taggit_mode == TAGGIT_TAG) {
        struct taglist *ptr;

        ptr = tags_head;
        while (ptr != NULL) {
            if (ptr->type == TAG_INT) {
                printf("[%s|%u] [%u]\n", ptr->name, ptr->id, ptr->integer);
            } else if (ptr->type == TAG_NUKE) {
                printf("[%s|%u] <-NUKE->\n", ptr->name, ptr->id);
            } else {
                printf("[%s|%u] [%s]\n", ptr->name, ptr->id, ptr->string);
            }
            ptr = ptr->next;
        }
    }
#endif
    first = true;
    for (i = optind; i < argc; ++i) {
        switch (taggit_mode) {
        case TAGGIT_LIST_HUMAN:
            if (!first)
                printf("\n");
            else
                first = false;
            taggit_list_human(argv[i]);
            break;
        case TAGGIT_LIST_MACHINE:
            if (!first)
                printf("%c", ASCII_EOT);
            else
                first = false;
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
