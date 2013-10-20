#include <iostream>
#include <map>

#include "tag.h"
#include "taggit.h"

static std::map< std::string,
                 std::pair< enum tag_id,
                            enum tag_type > >
tag_map = {
    { "artist",         { T_ARTIST,         TAG_STRING  } },
    { "album",          { T_ALBUM,          TAG_STRING  } },
    { "comment",        { T_COMMENT,        TAG_STRING  } },
    { "compilation",    { T_COMPILATION,    TAG_STRING  } },
    { "genre",          { T_GENRE,          TAG_STRING  } },
    { "tracknumber",    { T_TRACKNUMBER,    TAG_INTEGER } },
    { "tracktitle",     { T_TRACKTITLE,     TAG_STRING  } },
    { "year",           { T_YEAR,           TAG_INTEGER } }
};

/** Print a list of supported tags to ‘stdout‘. */
void
list_tags(void)
{
    for (auto iter = tag_map.begin(); iter != tag_map.end(); ++iter)
        std::cout << iter->first << std::endl;
}
