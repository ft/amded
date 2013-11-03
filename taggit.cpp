/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file taggit.cpp
 * @brief taggit's main() and command line option handling.
 */

#include <cstdlib>
#include <iostream>

#include "cmdline.h"
#include "file-spec.h"
#include "info.h"
#include "list-human.h"
#include "list-machine.h"
#include "setup.h"
#include "strip.h"
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
    TAGGIT_TAG,
    /** Remove all tags from a file */
    TAGGIT_STRIP
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
        std::cout << PROJECT
                  << ": -m, -l, -t and -t may *not* be used at the same time."
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

static void
verify_tag_name(enum tag_type type, const std::string &name)
{
    if (type == TAG_INVALID) {
        std::cerr << PROJECT << ": Invalid tag name: " << '"' << name << '"'
                  << std::endl;
        exit(EXIT_FAILURE);
    }
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

    while ((opt = bsd_getopt(argc, argv, "d:hLlmo:R:Sst:VW:")) != -1) {
        switch (opt) {
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
        case 'o':
            taggit_parameters(optarg);
            break;
        case 'R':
            setup_readmap(optarg);
            break;
        case 's':
            std::cout << "Supported tags:" << std::endl;
            list_tags();
            exit(EXIT_SUCCESS);
        case 'd':
            /* ‘-d’ is a special case of the TAGGIT_TAG mode. */
            check_mode_tag(taggit_mode);
            taggit_mode = TAGGIT_TAG;
            type = tag_to_type(optarg);
            verify_tag_name(type, optarg);
            tagval.set_invalid();
            add_tag(tag_to_id(optarg), tagval);
            break;
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
            verify_tag_name(type, tag.first);

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
            break;
        case 'S':
            check_mode(taggit_mode);
            taggit_mode = TAGGIT_STRIP;
            break;
        case 'V':
            taggit_version();
            exit(EXIT_SUCCESS);
        case 'W':
            setup_writemap(optarg);
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

    if (taggit_mode == TAGGIT_LIST_MACHINE ||
        taggit_mode == TAGGIT_LIST_HUMAN)
    {
        if (read_map.size() == 0)
            setup_readmap("");
    } else if (taggit_mode == TAGGIT_TAG) {
        if (write_map.size() == 0)
            setup_writemap("");
    }

    bool first = true;
    for (int i = optind; i < argc; ++i) {
        struct taggit_file file;
        file.name = argv[i];
        file.type = get_ext_type(argv[i]);
        if (file.type.get_id() == FILE_T_INVALID) {
            std::cerr << PROJECT ": Unsupported filetype: `"
                      << file.name << "'" << std::endl;
            continue;
        }
        if (!taggit_open(file))
            continue;
        switch (taggit_mode) {
        case TAGGIT_LIST_HUMAN:
            if (!first)
                std::cout << std::endl;
            else
                first = false;
            taggit_list_human(file);
            break;
        case TAGGIT_LIST_MACHINE:
            if (!first)
                std::cout << ASCII_EOT;
            else
                first = false;
            taggit_list_machine(file);
            break;
        case TAGGIT_TAG:
            taggit_tag(file);
            break;
        case TAGGIT_STRIP:
            taggit_strip(file);
            break;
        default:
            std::cout << "Please use one action option (-m, -l, -t or -S)."
                      << std::endl;
            return EXIT_FAILURE;
        }
        delete file.fh;
    }

    return EXIT_SUCCESS;
}
