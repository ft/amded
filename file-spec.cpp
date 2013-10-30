/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file file-spec.cpp
 * @brief File specific routines and abstraction
 */

#include <algorithm>
#include <map>
#include <string>

#include "taggit.h"

static std::map < std::string, enum file_type >
file_type_map = {
    { "flac", FILE_T_OGG_FLAC },
    { "flc",  FILE_T_OGG_FLAC },
    { "mp3",  FILE_T_MP3 },
    { "ogg",  FILE_T_OGG_VORBIS },
    { "oga",  FILE_T_OGG_VORBIS }
};

static std::map < enum file_type, std::string >
file_type_map_reverse = {
    { FILE_T_OGG_FLAC,   "ogg-flac" },
    { FILE_T_OGG_VORBIS, "ogg-vorbis" },
    { FILE_T_MP3,        "mp3" }
};

std::string
get_file_type_reverse(enum file_type type)
{
    auto rv = file_type_map_reverse.find(type);
    if (rv == file_type_map_reverse.end())
        return "unknown-filetype";
    return rv->second;
}

enum file_type
get_file_type(std::string filename)
{
    int dotidx = filename.rfind(".");
    std::string ext = filename.substr(dotidx + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    auto rv = file_type_map.find(ext);
    if (rv == file_type_map.end())
        return FILE_T_INVALID;
    return rv->second;
}
