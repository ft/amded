/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

#ifndef INC_TAGLIB_EXT_H
#define INC_TAGLIB_EXT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "taggit.h"

typedef struct { int dummy; } TagLib_MPEG_File;

enum TagLib_MP3Tag_Type {
    MP3_NO_TAGS = 0,
    MP3_ID3V1 = 1,
    MP3_ID3V2 = 2,
    MP3_APE = 4,
    MP3_ALLTAGS = 7
};

void mp3_dotheape(TagLib_File *);
int mp3_strip(TagLib_MPEG_File *, int);
int mp3_type2taglib(int);

struct taggit_file taggit_file_open(const char *);
void taggit_file_destroy(struct taggit_file *);
int taggit_file_save(struct taggit_file *);

#ifdef __cplusplus
}
#endif

#endif /* INC_TAGLIB_EXT_H */
