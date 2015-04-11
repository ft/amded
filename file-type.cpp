/*
 * Copyright (c) 2013-2015 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file file-type.h
 * @brief Transparent file-type id vs. label handling
 *
 * Internally, it's convenient to use intergers to identify different types of
 * a thing. When interfacing with humans, strings are preferable for obvious
 * reasons. Having to manually convert from one to another is bothersome. Just
 * tracking both at the same time and keeping both at sync at the same time
 * lifts that burden.
 */

#include <map>
#include <string>

#include "file-spec.h"
#include "file-type.h"

using stdstring = std::string;

static std::map < enum file_type, std::string >
file_type_map = {
    { FILE_T_FLAC,       "flac" },
    { FILE_T_OGG_VORBIS, "ogg-vorbis" },
    { FILE_T_MP3,        "mp3" },
    { FILE_T_M4A,        "m4a" },
    { FILE_T_INVALID,    "invalid" }
};

namespace Amded {

    FileType::FileType()
    {
        label = "invalid";
        id = FILE_T_INVALID;
    }

    FileType::~FileType()
    {
    }

    FileType::FileType(std::string l)
    {
        for (auto &iter : file_type_map) {
            if (iter.second == l) {
                label = l;
                id = iter.first;
                return;
            }
        }
        id = FILE_T_INVALID;
        label = "invalid";
    }

    FileType::FileType(enum file_type t)
    {
        label = file_type_map[t];
        id = t;
    }

    FileType&
    FileType::operator=(std::string l)
    {
        for (auto &iter : file_type_map) {
            if (iter.second == l) {
                label = l;
                id = iter.first;
                return *this;
            }
        }
        id = FILE_T_INVALID;
        label = "invalid";
        return *this;
    }

    FileType&
    FileType::operator=(enum file_type t)
    {
        label = file_type_map[t];
        id = t;
        return *this;
    }

    std::string
    FileType::get_label(void) const
    {
        return label;
    }

    enum file_type
    FileType::get_id(void) const
    {
        return id;
    }

}
