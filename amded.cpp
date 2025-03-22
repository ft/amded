/*
 * Copyright (c) 2013-2025 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file amded.cpp
 * @brief amded's main() and command line option handling.
 */

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "amded.h"
#include "cmdline.h"
#include "file-spec.h"
#include "info.h"
#include "list-human.h"
#include "list-json.h"
#include "list-machine.h"
#include "mode.h"
#include "setup.h"
#include "strip.h"
#include "tag.h"

#include "bsdgetopt.c"

/**
 * ASCII End-Of-Transmission character code. Used to seperate output for
 * different files in machine readable mode.
 */
#define ASCII_EOT ((char)0x04)

/* Short-hand to address items from enum class OperationMode */
using AmdedMode = Amded::OperationMode;

/** global variable describing amded's operation mode */
static Amded::Mode amded_mode{};

static void
amded_failure(void)
{
    std::cout << PROJECT
              << ": -m, -j, -l and -t/-d may *not* be used at the same time.\n";
    exit(EXIT_FAILURE);
}

/**
 * Check that -m, -l and -t are not used with one another
 *
 * @return      void
 * @sideeffects Exists with EXIT_FAILURE on failure.
 */
static inline void
check_singlemode_ok(void)
{
    if (!amded_mode.singlemode_ok()) {
        amded_failure();
    }
}

/**
 * Mode sanity check for tagging mode
 *
 * @return      void
 * @sideeffects see check_singlemode_ok()
 */
static inline void
check_multimode_ok(void)
{
    if (!amded_mode.multimode_ok()) {
        amded_failure();
    }
}

static void
verify_tag_name(enum tag_type type, const std::string &name)
{
    if (type == TAG_INVALID) {
        std::cerr << PROJECT << ": Invalid tag name: " << '"' << name << "\"\n";
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
 * amded_mode accordingly and let main()'s main loop handle everything.
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

    while ((opt = bsd_getopt(argc, argv, "d:hjLlmo:R:Ss:t:VW:")) != -1) {
        switch (opt) {
        case 'h':
            amded_usage();
            exit(EXIT_SUCCESS);
        case 'j':
            check_singlemode_ok();
            amded_mode.set(AmdedMode::LIST_JSON);
            break;
        case 'L':
            amded_licence();
            exit(EXIT_SUCCESS);
        case 'l':
            check_singlemode_ok();
            amded_mode.set(AmdedMode::LIST_HUMAN);
            break;
        case 'm':
            check_singlemode_ok();
            amded_mode.set(AmdedMode::LIST_MACHINE);
            break;
        case 'o':
            amded_parameters(optarg);
            break;
        case 'R':
            setup_readmap(optarg);
            break;
        case 's':
            if (strcmp(optarg, "tags") == 0) {
                list_tags();
            } else if (strcmp(optarg, "file-extensions") == 0) {
                list_extensions();
            } else {
                std::cerr << PROJECT << ": Unknown aspect `"
                          << optarg << "'." << std::endl;
            }
            exit(EXIT_SUCCESS);
        case 'd':
            /* ‘-d’ is a special case of the TAG mode. */
            check_multimode_ok();
            amded_mode.set(AmdedMode::TAG);
            type = tag_to_type(optarg);
            verify_tag_name(type, optarg);
            tagval.set_invalid();
            add_tag(tag_to_id(optarg), tagval);
            break;
        case 't':
            check_multimode_ok();
            amded_mode.set(AmdedMode::TAG);

            /* First check if the definition looks like "foo=bar" */
            try {
                tag = tag_arg_to_pair(optarg);
            }
            catch (amded_broken_tag_def) {
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
            unset_only_tag_delete();
            break;
        case 'S':
            check_singlemode_ok();
            amded_mode.set(AmdedMode::STRIP);
            break;
        case 'V':
            amded_version();
            exit(EXIT_SUCCESS);
        case 'W':
            setup_writemap(optarg);
            break;
        default:
            amded_usage();
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * amded: command line utility for listing and modifying meta
 *         information in audio files
 *
 * Interfacing KDE's taglib:
 *   <http://taglib.github.io>
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
        amded_usage();
        return EXIT_FAILURE;
    }

    parse_options(argc, argv);

    if (optind == argc) {
        amded_usage();
        return EXIT_FAILURE;
    }

    if (amded_mode.is_list_mode()) {
        if (read_map.empty()) {
            setup_readmap("");
        }
    } else if (amded_mode.is_write_mode()) {
        if (write_map.empty()) {
            setup_writemap("");
        }
    }

    bool first = true;
    for (int i = optind; i < argc; ++i) {
        struct amded_file file;
        file.name = argv[i];
        file.type = get_ext_type(argv[i]);
        if (file.type.get_id() == FILE_T_INVALID) {
            std::cerr << PROJECT ": Unsupported filetype: `"
                      << file.name << "'" << std::endl;
            continue;
        }
        if (!amded_open(file)) {
            continue;
        }
        switch (amded_mode.get()) {
        case AmdedMode::LIST_HUMAN:
            if (!first) {
                std::cout << std::endl;
            } else {
                first = false;
            }
            amded_list_human(file);
            break;
        case AmdedMode::LIST_JSON:
            if (first) {
                first = false;
            }
            amded_push_json(file);
            break;
        case AmdedMode::LIST_MACHINE:
            if (!first) {
                std::cout << ASCII_EOT;
            } else {
                first = false;
            }
            amded_list_machine(file);
            break;
        case AmdedMode::TAG:
            amded_tag(file);
            break;
        case AmdedMode::STRIP:
            amded_strip(file);
            break;
        default:
            std::cout << "Please use one action option (-m, -l, -t or -S)."
                      << std::endl;
            return EXIT_FAILURE;
        }
        delete file.fh;
    }

    if (amded_mode.get() == AmdedMode::LIST_JSON) {
        amded_list_json();
    }

    return EXIT_SUCCESS;
}
