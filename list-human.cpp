/*
 * Copyright (c) 2013-2025 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file list-human.cpp
 * @brief Tag reader frontend for humans
 */

#include <iomanip>
#include <iostream>
#include <map>
#include <string>

#include <fileref.h>
#include <tpropertymap.h>

#include "amded.h"
#include "list-human.h"
#include "list.h"
#include "value.h"

static void
print_iter(const std::pair< const std::string, Value > &iter, bool symbol=false)
{
    std::cout << std::setw(AMDED_TAG_MAXLENGTH)
              << std::left
              << iter.first
              << " | ";
    if (iter.second.get_type() == TAG_INTEGER) {
        std::cout << iter.second.get_int();
    } else if (iter.second.get_type() == TAG_BOOLEAN) {
        std::cout << (iter.second.get_bool() ? "true" : "false");
    } else if (iter.second.get_type() == TAG_STRING) {
        if (symbol) {
            std::cout << iter.second.get_str().toCString(true);
        } else {
            std::cout << '"'
                      << iter.second.get_str().toCString(true)
                      << '"';
        }
    } else {
        std::cout << "<INVALID DATA>";
    }
    std::cout << std::endl;
}

void
amded_list_human(const struct amded_file &file)
{
    std::cout << '<' << file.name << '>' << std::endl;

    std::map< std::string, Value > data = amded_list_amded(file);
    for (auto &iter : data) {
        print_iter(iter, true);
    }

    data = amded_list_tags(file);
    for (auto &iter : data) {
        print_iter(iter);
    }

    data = amded_list_audioprops(file.fh->audioProperties());
    for (auto &iter : data) {
        print_iter(iter);
    }
}
