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

#include <fileref.h>

#include "list.h"
#include "taggit.h"
#include "value.h"

static void
print_iter(std::pair< const std::string, Value > &iter)
{
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

void
taggit_list_human(char *file)
{
    std::cout << '<' << file << '>' << std::endl;

    TagLib::FileRef fr(file);
    if (fr.isNull() || !fr.tag() || !fr.audioProperties())
        return;

    std::map< std::string, Value > data = taggit_list_tags(fr);
    for (auto &iter : data)
        print_iter(iter);

    data = taggit_list_audioprops(fr);
    for (auto &iter : data)
        print_iter(iter);
}
