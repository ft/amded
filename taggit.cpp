/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 *
 * Terms for redistribution and use can be found in doc/LICENCE.
 */

/**
 * @file taggit.cpp
 * @brief taggit's main() and command line option handling.
 */

#include <cstdlib>
#include <cstdint>
#include <iostream>

#include "info.h"
#include "tag.h"
#include "taggit.h"

#include "bsdgetopt.c"

/**
 * ASCII End-Of-Transmission character code. Used to seperate output for
 * different files in machine readable mode.
 */
#define ASCII_EOT ((char)0x04)

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

/** option bit mask to alter taggit execution behaviour */
uint32_t taggit_options = 0;

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
        std::cout << "-m, -l and -t may *not* be used at the same time."
                  << std::endl;
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
parse_options(int argc, char *argv[])
{
    int opt;
    std::pair<std::string, std::string> tag;
    enum tag_type type;
    Value tagval;

    while ((opt = bsd_getopt(argc, argv, "EhLlmR:st:VW:")) != -1) {
        switch (opt) {
        case 'E':
            SET_OPT(TAGGIT_LIST_ALLOW_EMPTY_TAGS);
            break;
        case 'h':
            taggit_usage();
            exit(EXIT_SUCCESS);
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
        case 'R':
            //setup_readmap(optarg);
#ifdef TAGGIT_DEBUG
            //dump_readmap();
#endif
            break;
        case 's':
            std::cout << "Supported tags:" << std::endl;
            list_tags();
            exit(EXIT_SUCCESS);
        case 't':
            check_mode_tag(taggit_mode);
            taggit_mode = TAGGIT_TAG;

            /* First check if the definition looks like "foo=bar" */
            try {
                tag = tag_arg_to_pair(optarg);
            }
            catch (taggit_broken_tag_def) {
                std::cerr << PROJECT << ": Broken tag definition: "
                          << '"' << optarg << '"'
                          << std::endl;
                exit(EXIT_FAILURE);
            }

            /* Make sure ‘foo’ in "foo=bar" is a supported tag name */
            type = tag_to_type(tag.first);
            if (type == TAG_INVALID) {
                std::cerr << PROJECT << ": Invalid tag name: "
                          << '"' << tag.first << '"'
                          << std::endl;
                exit(EXIT_FAILURE);
            }

            /* Make sure ‘bar’ in "foo=bar" makes sense as a value for ‘foo’ */
            tagval = tag_value_from_value(type, tag.second);
            if (tagval.get_type() == TAG_INVALID) {
                std::cerr << PROJECT << ": Invalid tag value ["
                          << tag.second
                          << "] for tag " << '"'
                          << tag.first
                          << '"' << '!'
                          << std::endl;
                exit(EXIT_FAILURE);
            }

            /* Looks good. Add the tag. */
            add_tag(tag_to_id(tag.first), tagval);

#ifdef TAGGIT_DEBUG
            std::cerr << '[' << tag.first << ']'
                      << ' '
                      << '{' << tag.second << '}'
                      << ' '
                      << '<' << type << '>'
                      << std::endl;
#endif
            break;

        case 'V':
            taggit_version();
            exit(EXIT_SUCCESS);
        case 'W':
            //setup_writemap(optarg);
#ifdef TAGGIT_DEBUG
            //dump_writemap();
#endif
            break;
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
main(int argc, char *argv[])
{
    if (argc < 2) {
        taggit_usage();
        return EXIT_FAILURE;
    }

    parse_options(argc, argv);

    if (optind == argc) {
        taggit_usage();
        return EXIT_FAILURE;
    }

    bool first = true;
    for (int i = optind; i < argc; ++i) {
        switch (taggit_mode) {
        case TAGGIT_LIST_HUMAN:
            if (!first)
                std::cout << std::endl;
            else
                first = false;
            // taggit_list_human(argv[i]);
            break;
        case TAGGIT_LIST_MACHINE:
            if (!first)
                std::cout << ASCII_EOT;
            else
                first = false;
            // taggit_list_machine(argv[i]);
            break;
        case TAGGIT_TAG:
            // taggit_tag(argv[i]);
            break;
        default:
            std::cout << "Please use one action option (-m, -l or -t)."
                      << std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
