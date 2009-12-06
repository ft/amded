/*
 * Copyright (c) 2009 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file  setup.c
 * @brief configuration code
 */

#include <stdlib.h>
#include <stdio.h>

#include "taggit.h"
#include "taglib_ext.h"

/** local data type to map names to tag type ids */
struct typemap_ {
    char *name;
    unsigned int id;
};

/** mapping strings to mp3 tag type ids */
static struct typemap_ mp3_map[] = {
    { "apetag", MP3_APE },
    { "id3v1", MP3_ID3V1 },
    { "id3v2", MP3_ID3V2 },
    { (char*)NULL, 0 }
};

/**
 * The readmap
 *
 * If files support multiple different types of tags in one file,
 * this configures in which order the types are tried. These are the
 * default values, which the -R switch can change.
 *
 * This uses a winner takes it all approach. If for example, an idv2
 * tag exists for mp3s, *all* information is read from it. The other
 * tag types, even if they exist, are *not* even considered.
 *
 * Currently only implemented for mp3 files.
 */
static struct {
    char *name;
    unsigned int ids[TAGGIT_MAP_MAX];
    struct typemap_ *tags;
} readmap[] = {
    { "mp3", { MP3_ID3V2, MP3_APE, MP3_ID3V1 }, mp3_map },
    { (char*)NULL, { 0, 0, 0 }, (struct typemap_ *)NULL }
};

/**
 * Find a readmap[] entry by name
 *
 * @param   name    the name of the map to look for
 *
 * @return      the index of the matched entry; or the index
 *              of the invalid entry at the end of the map.
 * @sideeffects none
 */
static int
find_readmap(char *name)
{
    int i;

    for (i = 0; readmap[i].name != NULL; ++i)
        if (strcaseeq(name, readmap[i].name))
            return i;

    fprintf(stderr, "Unknown map name (%s).\n", name);
    return i;
}

/**
 * Find a tag type entry in a given typemap_ (by name)
 *
 * @param   name    the name of the tag type to look for
 * @param   map     the typemap_ in which to do the lookup
 *
 * @return      the id of the matched entry; 0 if nothing was matched
 * @sideeffects none
 */
static unsigned int
find_tagtype(char *name, struct typemap_ *map)
{
    int i;

    for (i = 0; map[i].name != NULL; ++i)
        if (strcaseeq(name, map[i].name))
            return map[i].id;

    fprintf(stderr, "Unknown tag type (%s).\n", name);
    return 0;
}

/**
 * Change the readmap
 *
 * The map that will be changed need to have been determined already.
 * Then, this function accepts a comma seperated list of tag types, that
 * makes up the mapping.
 *
 * @param   map     the index of the mapping to change in readmap[]
 * @param   s       a string; the comma seperated tag type list
 *
 * @return      true one success; false otherwise
 * @sideeffects none
 */
static boolean
change_readmap(int map, char *s)
{
    char *ptr;
    int i;

    ptr = s;
    i = 0;
    while (ptr != NULL) {
        if (i > TAGGIT_MAP_MAX - 1) {
            fprintf(stderr, "Too many tag types, defined (max: %d).\n",
                    TAGGIT_MAP_MAX);
            return false;
        }

        ptr = strchr(ptr, (int)',');
        if (ptr != NULL)
            *(ptr++) = '\0';

        readmap[map].ids[i] = find_tagtype(s, readmap[map].tags);
        if (readmap[map].ids[i] == 0)
            return false;

        s = ptr;
        ++i;
    }

    return true;
}

/**
 * The readmap configuration frontend
 *
 * Takes an argument that looks like this:
 *
 * filetypeA=tagtypeA,tagtypeB:filetypeBtagtypeC,...
 *
 * This splits up that argument into single filetype=... mappings.
 *
 * @param   s       the optarg from getopt (see above)
 *
 * @return      void
 * @sideeffects exits EXIT_FAILURE if an invalid readmap was detected.
 */
void
setup_readmap(const char *s)
{
    char *ptr, *sc;
    int map;
    boolean rc;

    ptr = (char *)s;
    while (ptr != NULL && *ptr != '\0') {
        sc = strchr(ptr, (int)'=');
        if (sc == NULL)
            goto err;

        *(sc++) = '\0';
        map = find_readmap(ptr);
        if (readmap[map].name == NULL)
            goto err;

        rc = change_readmap(map, sc);
        if (!rc)
            goto err;

        sc = strchr(sc, (int)':');
        if (sc == NULL)
            ptr = NULL;
        else {
            *sc = '\0';
            ptr = sc + 1;
        }
    }
    return;

err:
    fprintf(stderr, "Invalid readmap.\n");
    exit(EXIT_FAILURE);
}
