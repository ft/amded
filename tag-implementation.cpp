/*
 * Copyright (c) 2013-2014 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file tag-implemenation.cpp
 * @brief Transparent tag-implementation-type id vs. label handling
 */

#include <map>
#include <string>

#include "tag-implementation.h"

using stdstring = std::string;

static std::map< enum tag_impl, std::string > tagimpl_map {
    { TAG_T_APETAG,  "apetag" },
    { TAG_T_ID3V1,   "id3v1" },
    { TAG_T_ID3V2,   "id3v2" },
    { TAG_T_NONE,    "none" },
    { TAG_T_INVALID, "invalid" },
};

namespace Amded {

    TagImplementation::TagImplementation()
    {
        label = "invalid";
        id = TAG_T_INVALID;
    }

    TagImplementation::~TagImplementation()
    {
    }

    TagImplementation::TagImplementation(std::string l)
    {
        for (auto &iter : tagimpl_map) {
            if (iter.second == l) {
                label = l;
                id = iter.first;
                return;
            }
        }
        id = TAG_T_INVALID;
        label = "invalid";
    }

    TagImplementation::TagImplementation(enum tag_impl t)
    {
        label = tagimpl_map[t];
        id = t;
    }

    TagImplementation&
    TagImplementation::operator=(std::string l)
    {
        for (auto &iter : tagimpl_map) {
            if (iter.second == l) {
                label = l;
                id = iter.first;
                return *this;
            }
        }
        id = TAG_T_INVALID;
        label = "invalid";
        return *this;
    }

    TagImplementation&
    TagImplementation::operator=(enum tag_impl t)
    {
        label = tagimpl_map[t];
        id = t;
        return *this;
    }

    std::string
    TagImplementation::get_label(void) const
    {
        return label;
    }

    enum tag_impl
    TagImplementation::get_id(void) const
    {
        return id;
    }

}
