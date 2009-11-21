/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#include <taglib/tag_c.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>

#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/vorbisfile.h>

#include "taggit.h"
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

/*
 * Open files in their specific types, if we know them.
 * Otherwise, use TagLibs generic interface only. That will stop
 * us from working with multi-artist files for those types, but
 * it's better than nothing.
 */
struct taggit_file
taggit_file_open(const char *file)
{
    struct taggit_file rc;
    char *ext;

    ext = strrchr(file, (int)'.');
    if (ext == NULL || *(ext + 1) == '\0')
        goto err;

    ext++;
    if (strcaseeq(ext, (char *)"mp3")) {
        rc.data = reinterpret_cast<TagLib_File *>
            (new TagLib::MPEG::File(file));
        rc.type = FT_MPEG;
    } else if (strcaseeq(ext, (char *)"ogg")) {
        rc.data = reinterpret_cast<TagLib_File *>
            (new TagLib::Ogg::Vorbis::File(file));
        rc.type = FT_OGGVORBIS;
    } else if (strcaseeq(ext, (char *)"oga")) {
        TagLib::File *f;

        f = new TagLib::Ogg::FLAC::File(file);
        if (f->isValid()) {
            rc.data = reinterpret_cast<TagLib_File *>(f);
            rc.type = FT_OGGFLAC;
        } else {
            delete f;
            rc.data = reinterpret_cast<TagLib_File *>
                (new TagLib::Ogg::Vorbis::File(file));
            rc.type = FT_OGGVORBIS;
        }
    } else if (strcaseeq(ext, (char *)"flac")) {
        rc.data = reinterpret_cast<TagLib_File *>
            (new TagLib::FLAC::File(file));
        rc.type = FT_FLAC;
    } else {
        TagLib_File *f;

        f = taglib_file_new(file);
        if (!taglib_file_is_valid(f)) {
            taglib_file_free(f);
            goto err;
        } else {
            rc.data = f;
            rc.type = FT_UNKNOWN;
        }
    }

    return rc;

err:
    fprintf(stderr, "Cannot handle file: \"%s\" - skipping.\n", file);
    rc.data = NULL;
    rc.type = FT_INVALID;
    return rc;
}

void
taggit_file_destroy(struct taggit_file *file)
{
    taglib_file_free(file->data);
    file->type = FT_INVALID;
}
