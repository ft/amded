/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#include <taglib/mpegfile.h>

#include "taglib_ext.h"

int
mp3_type2taglib(int mask)
{
    int rc;

    if (mask == MP3_NO_TAGS)
        return TagLib::MPEG::File::NoTags;
    if (mask == MP3_ALLTAGS)
        return TagLib::MPEG::File::AllTags;

    rc = 0;
    if (mask & MP3_ID3V1)
        rc |= TagLib::MPEG::File::ID3v1;
    if (mask & MP3_ID3V2)
        rc |= TagLib::MPEG::File::ID3v2;
    if (mask & MP3_APE)
        rc |= TagLib::MPEG::File::APE;

    return rc;
}

int
mp3_strip(TagLib_MPEG_File *f, int mask)
{
    TagLib::MPEG::File::File *file;

    file = reinterpret_cast<TagLib::MPEG::File::File *>(f);
    return file->strip(mp3_type2taglib(mask));
}
