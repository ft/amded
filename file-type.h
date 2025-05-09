/*
 * Copyright (c) 2013-2025 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file file-type.h
 * @brief API for transparent file-type id vs. label handling
 */

#ifndef INC_FILE_TYPE_H
#define INC_FILE_TYPE_H

#include <string>

enum file_type {
    FILE_T_INVALID = -1,
    FILE_T_M4A,
    FILE_T_MP3,
    FILE_T_FLAC,
    FILE_T_OGG_VORBIS,
    FILE_T_OPUS
};

namespace Amded {

    class FileType {
    private:
        enum file_type id;
        std::string label;

    public:
        /* Default {de,con}struction */
        FileType();
        ~FileType();

        /* ...via id */
        FileType(enum file_type);
        FileType& operator=(enum file_type);

        /* ...via string */
        FileType(const std::string&);
        FileType& operator=(const std::string&);

        std::string get_label() const;
        enum file_type get_id() const;
    };

}

#endif /* INC_FILE_TYPE_H */
