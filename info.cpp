/*
 * Copyright (c) 2009-2013 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  info.cpp
 * @brief display various informational messages
 */

#include <iostream>
#include <vector>

#include "taggit.h"
#include "info.h"

#ifdef VENDOR_BUILD
#include "taggit-vendor.h"
#endif /* VENDOR_BUILD */

#ifdef GIT_SOURCE
#include "git-version.h"
#endif /* GIT_SOURCE */

/** usage information */
std::vector<std::string> usage = {
"usage: taggit OPTION(s) FILE(s)",
"",
"  informational options:",
"    -h,               display this help text",
"    -L,               show taggit's licence information",
"    -v,               print version information",
"    -s                list supported tag names",
"  configuration options:",
"    -R <readmap>      configure tag reading order",
"    -W <writemap>     configure which tag types should be written",
"    -o <param-list>   pass in a comma-separated list of parameters",
"  action options:",
"    -l                list tags in human readable form",
"    -m                list tags in machine readable form",
"    -S                strip all tags from the file",
"    -t <tag>=<value>  set a tag to a value",
"    -d <tag>          delete a tag from the file",
};

/** licence information */
std::vector<std::string> licence = {
" Copyright 2009-2013 taggit workers, All rights reserved.",
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
};

/** Print version information about taggit */
void
taggit_version(void)
{
    std::cout << PROJECT << " version ";
#ifdef GIT_SOURCE
    std::cout << GIT_VERSION;
#else
    std::cout << VERSION;
#endif /* GIT_SOURCE */

#ifdef VENDOR_BUILD
    std::cout << " (" << VENDOR_VERSION << ")";
#endif /* VENDOR_VERSION */
    std::cout << std::endl;

#ifdef GIT_SOURCE
    std::cout << "Based on commit: " << GIT_DESCRIPTION << std::endl;
#endif /* GIT_SOURCE */
}

static void
print_vector(std::vector<std::string> v)
{
    for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it)
        std::cout << *it << std::endl;
}

/** Print out usage information (contained in the usage vector above) */
void
taggit_usage(void)
{
    print_vector(usage);
}

/** Print out licencing information (contained in the licence vector above) */
void
taggit_licence(void)
{
    print_vector(licence);
}
