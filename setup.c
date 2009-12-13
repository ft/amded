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

/** local data type to represent read and write maps */
struct rw_map {
    char *name;
    unsigned int ids[TAGGIT_MAP_MAX];
    struct typemap_ *tags;
};

/** local data type to map file type ids to bit masks */
struct mask_map {
    enum file_type type;
    int mask;
};

/** local data type to tell read an write map apart */
enum map_type {
    TAGGIT_UNKNOWN_MAP = 0,
    TAGGIT_READ_MAP,
    TAGGIT_WRITE_MAP
};

/** local data type to map names to tag type ids */
struct typemap_ {
    const char *name;
    int id;
};

/** mapping strings to mp3 tag type ids */
static struct typemap_ mp3_map[] = {
    { "apetag", MP3_APE },
    { "id3v1", MP3_ID3V1 },
    { "id3v2", MP3_ID3V2 },
    { (const char*)NULL, 0 }
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
static struct rw_map readmap[] = {
    { "mp3", { MP3_ID3V2, MP3_APE, MP3_ID3V1 }, mp3_map },
    { (char*)NULL, { 0, 0, 0 }, (struct typemap_ *)NULL }
};

/**
 * The writemap
 *
 * If files support multiple different types of tags in one file,
 * this configures in which order the types are tried. These are the
 * default values, which the -W switch can change.
 *
 * Tag values specified on the command line will be written out to
 * *all* tag types defined within this map.
 *
 * Currently only implemented for mp3 files.
 */
static struct rw_map writemap[] = {
    { "mp3", { MP3_ID3V2, MP3_NO_TAGS, MP3_NO_TAGS }, mp3_map },
    { (char*)NULL, { 0, 0, 0 }, (struct typemap_ *)NULL }
};

/**
 * Map write masks to file type ids
 *
 * This structure is setup by setup_create_write_masks() at run time.
 */
static struct mask_map write_masks[] = {
    { FT_MPEG, 0 },
    { FT_INVALID, 0 }
};

/**
 * Look up a human readable string to an mp3 tag type id
 *
 * @param   id      the id to look up
 *
 * @return      A pointer to the string in the mp3 tag type map, which
 *              matches the given id; A link to a "(no tag)" string if
 *              nothing matched.
 * @sideeffects none
 */
const char *
mp3_id_to_string(int id)
{
    int i;

    for (i = 0; mp3_map[i].name != NULL; ++i)
        if (mp3_map[i].id == id)
            return mp3_map[i].name;

    return "(no tag)";
}

/**
 * Translate enum map_type into a pointer to the associated map
 *
 * @param   id      the id to look up
 *
 * @return      pointer to the map associated to the given id
 * @sideeffects exits EXIT_FAILURE if an invalid map was detected.
 */
static struct rw_map *
mapid_to_rwmap(int id)
{
    switch (id) {
    case TAGGIT_READ_MAP:
        return readmap;
    case TAGGIT_WRITE_MAP:
        return writemap;
    default:
        fprintf(stderr,
                "Fatal: Unknown map type (%d). Report this bug.\n", id);
        exit(EXIT_FAILURE);
    }
}

/**
 * Find a read or write map entry by name
 *
 * @param   type    the type of the map to work on (read or write)
 * @param   name    the name of the map to look for
 *
 * @return      the index of the matched entry; or the index
 *              of the invalid entry at the end of the map.
 * @sideeffects none
 */
static int
find_map(enum map_type type, char *name)
{
    struct rw_map *ptr;
    int i;

    ptr = mapid_to_rwmap(type);
    for (i = 0; ptr[i].name != NULL; ++i)
        if (strcaseeq(name, ptr[i].name))
            return i;

    fprintf(stderr, "Unknown map name (%s).\n", name);
    return i;
}

/**
 * Check which of a set of tag types to read
 *
 * Since tag type ids are ORable, types is such an ORed integer.
 * Consider the following:
 *
 * <code>
 *  types = 0;
 *  types |= MP3_APE;
 *  types |= MP3_ID3V2;
 * </code>
 *
 * This will check the read map for apetags and id3v2 tags and return the value
 * of the one that has higher priority.
 *
 * @param   mapname     string describing the read map to check
 *                      (e.g.: "mp3" for mp3 files)
 * @param   types       (possibly ORed) type tag id(s) to check for
 *
 * @return      The tag type id with the highest priority in the read map
 * @sideeffects none
 */
int
setup_please_read(char *mapname, int types)
{
    int i, idx;

    idx = find_map(TAGGIT_READ_MAP, mapname);
    for (i = 0; i < TAGGIT_MAP_MAX; ++i)
        if (types & readmap[idx].ids[i])
            return readmap[idx].ids[i];

    return -1;
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
        if (strcaseeq(name, (char *)map[i].name))
            return map[i].id;

    fprintf(stderr, "Unknown tag type (%s).\n", name);
    return 0;
}

/**
 * Change a mapping (read or write)
 *
 * The map that will be changed need to have been determined already.
 * Then, this function accepts a comma seperated list of tag types, that
 * makes up the mapping.
 *
 * @param   type    the type of the map to work on (read or write)
 * @param   idx     the index of the mapping to change in the selected map
 * @param   s       a string; the comma seperated tag type list
 *
 * @return      true one success; false otherwise
 * @sideeffects none
 */
static boolean
change_map(enum map_type type, int idx, char *s)
{
    struct rw_map *map;
    char *ptr;
    int i;

    ptr = s;
    i = 0;
    map = mapid_to_rwmap(type);
    while (ptr != NULL) {
        if (i > TAGGIT_MAP_MAX - 1) {
            fprintf(stderr, "Too many tag types, defined (max: %d).\n",
                    TAGGIT_MAP_MAX);
            return false;
        }

        ptr = strchr(ptr, (int)',');
        if (ptr != NULL)
            *(ptr++) = '\0';

        map[idx].ids[i] = find_tagtype(s, map[idx].tags);
        if (map[idx].ids[i] == 0)
            return false;

        s = ptr;
        ++i;
    }

    for (; i < TAGGIT_MAP_MAX; ++i)
        map[idx].ids[i] = 0;

    return true;
}

/**
 * The read/writemap configuration frontend
 *
 * Takes an argument that looks like this:
 *
 * filetypeA=tagtypeA,tagtypeB:filetypeBtagtypeC,...
 *
 * This splits up that argument into single filetype=... mappings.
 *
 * @param   type    the type of the map to work on (read or write)
 * @param   s       the optarg from getopt (see above)
 *
 * @return      void
 * @sideeffects exits EXIT_FAILURE if an invalid map was detected.
 */
static void
setup_map(enum map_type type, const char *s)
{
    struct rw_map *map;
    char *ptr, *sc;
    int mapidx;
    boolean rc;

    map = mapid_to_rwmap(type);
    ptr = (char *)s;
    while (ptr != NULL && *ptr != '\0') {
        sc = strchr(ptr, (int)'=');
        if (sc == NULL)
            goto err;

        *(sc++) = '\0';
        mapidx = find_map(type, ptr);
        if (map[mapidx].name == NULL)
            goto err;

        rc = change_map(type, mapidx, sc);
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
    if (type == TAGGIT_READ_MAP)
        fprintf(stderr, "Invalid readmap.\n");
    else
        fprintf(stderr, "Invalid writemap.\n");
    exit(EXIT_FAILURE);
}

/**
 * public interface to setup read maps
 *
 * See setup_map() for details.
 */
void
setup_readmap(const char *s)
{
    setup_map(TAGGIT_READ_MAP, s);
}

/**
 * public interface to setup write maps
 *
 * See setup_map() for details.
 */
void
setup_writemap(const char *s)
{
    setup_map(TAGGIT_WRITE_MAP, s);
}

/**
 * Get the write mask for a given file type
 *
 * @param   type    the file type id for which you want the write mask
 *
 * @return      the requested write mask; 0 is the type couldn't be matched
 * @sideeffects none
 */
int
setup_get_write_mask(enum file_type type)
{
    int i;

    for (i = 0; write_masks[i].type != FT_INVALID; ++i)
        if (type == write_masks[i].type)
            return write_masks[i].mask;

    return 0;
}

/**
 * Setup the write masks structure
 *
 * @return      void
 * @sideeffects none
 */
void
setup_create_write_masks(void)
{
    int mask, i, j;
    enum file_type type;

    mask = 0;
    for (i = 0; writemap[i].name != NULL; ++i) {
        type = get_file_type_id(writemap[i].name);
        for (j = 0; j < TAGGIT_MAP_MAX; ++j) {
            if (writemap[i].ids[j] == 0)
                continue;

            mask |= writemap[i].ids[j];
#ifdef TAGGIT_DEBUG
            fprintf(stderr, "new: %d, now: %d\n", writemap[i].ids[j], mask);
#endif
        }
        if (type == FT_MPEG) {
            mask = mp3_type2taglib(mask);
#ifdef TAGGIT_DEBUG
            fprintf(stderr, "mp3mask: %d\n", mask);
#endif
        }
        for (j = 0; write_masks[j].type != FT_INVALID; ++j) {
            if (type == write_masks[j].type)
                write_masks[j].mask = mask;
        }
    }
}

#ifdef TAGGIT_DEBUG
static void
dump_map(enum map_type type)
{
    struct rw_map *ptr;
    int i, j;

    ptr = mapid_to_rwmap(type);
    for (i = 0; ptr[i].name != NULL; ++i) {
        fprintf(stderr, "%s:\n", ptr[i].name);
        for (j = 0; j < TAGGIT_MAP_MAX; ++j) {
            fprintf(stderr, "    id: %d\n", ptr[i].ids[j]);
        }
    }
}

void
dump_readmap(void)
{
    fprintf(stderr, "= readmap =\n");
    dump_map(TAGGIT_READ_MAP);
}

void
dump_writemap(void)
{
    fprintf(stderr, "= writemap =\n");
    dump_map(TAGGIT_WRITE_MAP);
}
#endif
