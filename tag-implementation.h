/*
 * Copyright (c) 2013-201 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file tag-implemenation.h
 * @brief API for transparent tag-implementation-type id vs. label handling
 */

#ifndef INC_TAG_IMPLEMENTATION_H
#define INC_TAG_IMPLEMENTATION_H

#include <string>

enum tag_impl {
    TAG_T_INVALID = -1,
    TAG_T_APETAG,
    TAG_T_ID3V1,
    TAG_T_ID3V2,
    TAG_T_NONE
};

namespace Amded {

    class TagImplementation {
    private:
        enum tag_impl id;
        std::string label;

    public:
        /* Default {de,con}struction */
        TagImplementation();
        ~TagImplementation();

        /* ...via id */
        TagImplementation(enum tag_impl);
        TagImplementation& operator=(enum tag_impl);

        /* ...via string */
        TagImplementation(std::string);
        TagImplementation& operator=(std::string);

        std::string get_label() const;
        enum tag_impl get_id() const;
    };

}

#endif /* INC_TAG_IMPLEMENTATION_H */
