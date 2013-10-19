/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 *
 * Terms for redistribution and use can be found in doc/LICENCE.
 */

/**
 * @file taggit.cpp
 * @brief taggit's main()
 */

#include <cstdlib>
#include <iostream>
#include <boost/program_options.hpp>

#include "info.h"

namespace po = boost::program_options;

int
main(int argc, char *argv[])
{
    try {
        po::options_description desc("Options");
        desc.add_options()
            ("help,h", "Print help message.")
            ("licence,L", "Print licencing information")
            ("version,v", "Print version information")
            ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            taggit_usage();
            return EXIT_SUCCESS;
        }
        if (vm.count("version")) {
            taggit_version();
            return EXIT_SUCCESS;
        }
        if (vm.count("licence")) {
            taggit_licence();
            return EXIT_SUCCESS;
        }
    }

    catch(std::exception& e) {
        std::cerr << "fatal: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    catch(...) {
        std::cerr << "Unknown exception! (Crap.)\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
