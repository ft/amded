/*
 * Copyright (c) 2013-2015 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file list-machine.cpp
 * @brief Tag reader frontend for machines
 */

#include <iostream>
#include <map>
#include <string>

#include <fileref.h>
#include <tpropertymap.h>

#include "list.h"
#include "list-machine.h"
#include "value.h"

/** ascii start-of-text character code */
#define ASCII_STX ((char)0x02)

/** ascii end-of-text character code */
#define ASCII_ETX ((char)0x03)

static void
print_iter(std::pair< const std::string, Value > &iter)
{
    std::cout << ASCII_ETX << iter.first << ASCII_STX;
    if (iter.second.get_type() == TAG_INTEGER)
        std::cout << iter.second.get_int();
    else if (iter.second.get_type() == TAG_BOOLEAN)
        std::cout << (iter.second.get_bool() ? "true" : "false");
    else if (iter.second.get_type() == TAG_STRING)
        std::cout << iter.second.get_str().toCString(true);
}

void
amded_list_machine(const struct amded_file &file)
{
    std::cout << "file-name" << ASCII_STX << file.name;

    std::map< std::string, Value > data = amded_list_amded(file);
    for (auto &iter : data)
        print_iter(iter);

    data = amded_list_tags(file);
    for (auto &iter : data)
        print_iter(iter);

    data = amded_list_audioprops(file.fh->audioProperties());
    for (auto &iter : data)
        print_iter(iter);
}
