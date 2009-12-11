/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  taglib_ext.h
 * @brief API for extensions to taglib's generic C bindings
 */

#ifndef INC_TAGLIB_EXT_H
#define INC_TAGLIB_EXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <taglib/tag_c.h>

#include "taggit.h"

/**
 * MP3 tag type description enum accessible from C
 *
 * These values can be translated the TagLib's values via
 * mp3_type2taglib() from taglib_ext.cpp.
 */
enum TagLib_MP3Tag_Type {
    MP3_NO_TAGS = 0,
    MP3_ID3V1 = 1,
    MP3_ID3V2 = 2,
    MP3_APE = 4,
    MP3_ALLTAGS = 7
};

void mp3_dotheape(TagLib_File *);
int mp3_strip(TagLib_File *, int);
void mp3_tagtypes(TagLib_File *, struct taggit_list *);
int mp3_type2taglib(int);
int mp3_read_int(enum tag_id, TagLib_Tag *);
char *mp3_read_str(enum tag_id, TagLib_Tag *);

TagLib_Tag *taggit_file_tag(struct taggit_file *, int);
struct taggit_file taggit_file_open(const char *);
void taggit_file_destroy(struct taggit_file *);
int taggit_file_save(struct taggit_file *);

#ifdef __cplusplus
}
#endif

#endif /* INC_TAGLIB_EXT_H */
