/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  taglib_ext.cpp
 * @brief extensions for taglib's generic C bindings
 *
 * Taglib's C binings only support a limited feature set of what
 * taglib can actually do. It is very much a "do-what-I-mean" interface,
 * that support generic tags like: artist, album, tracktitle, -number,
 * year and genre.
 *
 * These are common to all supported file types; so that's reasonable.
 * It doesn't give the developer much control over file specific features,
 * like apetags in mp3 files. That is certainly something, that is desired
 * in taggit, so that needs extentions to taglib's generic interface.
 *
 * Also, taggit should transparently support multi-artist information from
 * audio files. That however is handled quite differently from file type
 * to file type. MP3 files could store it in TPE2 id3v2 frames or in an
 * apetag; ogg vorbis files may store it in ALBUMARTIST tags.
 *
 * That means, for multi-artist information, we'll need extensions to
 * taglib's C bindings, too.
 */

#include <errno.h>
#include <string.h>

#include <taglib/tag_c.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>

#include <taglib/apetag.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>

#include <taglib/flacfile.h>
#include <taglib/mpegfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/vorbisfile.h>

#include "taggit.h"
#include "taglib_ext.h"

/**
 * When make sure an apetag is there, so that saving a mp3 file back
 * can include apetags too.
 *
 * This is needed, since taglib will only create apetags in mp3 files
 * if that is explicitly requested.
 *
 * Only use this for FT_MPEG files. Other types may not have the required
 * method, which will cause fatal problems.
 *
 * @param   file    TagLib_File pointer to the mp3 in question.
 *
 * @return      void
 * @sideeffects none
 */
void
mp3_dotheape(TagLib_File *file)
{
    TagLib::MPEG::File *f;

    f = reinterpret_cast<TagLib::MPEG::File *>(file);
    f->APETag(true);
}

/**
 * Translate taggit's type description C enum to TagLib's type
 * description enum.
 *
 * @param   mask    the bitmask to translate
 *
 * @return      the translated bitmask
 * @sideeffects none
 */
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

/**
 * Create a string that lists which tag types were found in a mp3 file
 *
 * The return value is dynamically allocated and needs to be freed if
 * it is not in use anymore.
 *
 * @param   f       TagLib_File pointer to check
 *
 * @return      comma seperated list of found tag types
 * @sideeffects none
 */
char *
mp3_tagtypes(TagLib_File *f)
{
    TagLib::MPEG::File::File *file;
    TagLib::ID3v1::Tag *v1;
    TagLib::ID3v2::Tag *v2;
    TagLib::APE::Tag *ape;
    std::string tmp;
    char *rc;

    file = reinterpret_cast<TagLib::MPEG::File::File *>(f);
    tmp = "";
    v1 = file->ID3v1Tag();
    if (v1 != NULL && !v1->isEmpty())
        tmp = "id3v1";

    v2 = file->ID3v2Tag();
    if (v2 != NULL && !v2->isEmpty()) {
        if (tmp != "")
            tmp += ",";
        tmp += "id3v2";
    }
    ape = file->APETag();
    if (ape != NULL && !ape->isEmpty()) {
        if (tmp != "")
            tmp += ",";
        tmp += "apetag";
    }

    if (tmp == "")
        tmp = "(no tags)";

    rc = xstrdup(tmp.c_str());
    if (errno == ENOMEM) {
        fprintf(stderr, "Out of memory. Aborting.\n");
        exit(EXIT_FAILURE);
    }
    return rc;
}

/**
 * Strip one or more sets of tags in a mp3
 *
 * This can strip id3v1, v2 or apetags from a file.
 *
 * The mask needs to be the one, taglib understands, the ones from
 * our enum need to be translated using mp3_type2taglib():
 *
 * <code>
 *  mp3_strip(file, mp3_type2taglib(MP3_ID3V1 | MP3_APE));
 * </code>
 *
 * @param   f       TagLib_File pointer to check
 * @param   mask    bitmask describing the tags that should be stripped
 *
 * @return      comma seperated list of found tag types
 * @sideeffects none
 */
int
mp3_strip(TagLib_File *f, int mask)
{
    TagLib::MPEG::File::File *file;

    file = reinterpret_cast<TagLib::MPEG::File::File *>(f);
    return file->strip(mp3_type2taglib(mask));
}

/**
 * Open a file via taglib and add file type information
 *
 * Open files, that taglib supports but taggit doesn't have specific support
 * for using TagLibs generic interface only. That will stop us from working
 * with multi-artist files for those types, but it's better than nothing.
 *
 * @param   file    file name to open
 *
 * @return      copy of the generated struct taggit_file; *NOT* a pointer
 *              to it! Destroy using taggit_file_destroy().
 * @sideeffects none
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

/**
 * Free the allocated information from a taggit_file structure
 *
 * @param   file    pointer to the structure to destroy
 *
 * @return      void
 * @sideeffects none
 */
void
taggit_file_destroy(struct taggit_file *file)
{
    taglib_file_free(file->data);
    file->type = FT_INVALID;
}

/**
 * Save a file back to disc
 *
 * This is a wrapper around a file type's save() method, which
 * also handles file type specific actions.
 *
 * @param   file    pointer to the taggit_file structure describing
 *                  the file in question
 *
 * @return      1 if the save() method returned true; 0 otherwise
 * @sideeffects none
 */
int
taggit_file_save(struct taggit_file *file)
{
    bool rc;
    int mask;

    mask = TagLib::MPEG::File::ID3v2
         | TagLib::MPEG::File::APE;

    switch (file->type) {
    case FT_MPEG:
        TagLib::MPEG::File *f;

        f = reinterpret_cast<TagLib::MPEG::File *>(file->data);
        /**
         * Save id3v2 and apetag for now.
         * Throw away id3v1, which is utterly useless anyway.
         * @TODO This should probably be configurable...
         */
        rc = f->save(mask, 1);
        break;
    default:
        rc = taglib_file_save(file->data);
    }

    return rc ? 1 : 0;
}
