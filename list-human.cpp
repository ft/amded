/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file list-human.cpp
 * @brief Tag reader frontend for humans
 */

#include <map>
#include <string>
#include <iostream>
#include <iomanip>

#include "list.h"
#include "taggit.h"
#include "value.h"

void
taggit_list_human(char *file)
{
    std::map< std::string, Value > tags = taggit_list_tags(file);
    std::cout << '<' << file << '>' << std::endl;
    for (auto &iter : tags) {
        std::cout << std::setw(TAGGIT_TAG_MAXLENGTH)
                  << std::left
                  << iter.first
                  << " | ";
        if (iter.second.get_type() == TAG_INTEGER)
            std::cout << iter.second.get_int();
        else if (iter.second.get_type() == TAG_BOOLEAN)
            std::cout << (iter.second.get_bool() ? "true" : "false");
        else if (iter.second.get_type() == TAG_STRING)
            std::cout << '"'
                      << iter.second.get_str().toCString(true)
                      << '"';
        else
            std::cout << "<INVALID DATA>";
        std::cout << std::endl;
    }
}
