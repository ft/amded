/*
 * Copyright (c) 2013 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file strip.cpp
 * @brief Stripping all meta-data from files
 */

#include <iostream>

#include <fileref.h>
#include <tpropertymap.h>

#include "file-spec.h"
#include "strip.h"
#include "amded.h"

void
amded_strip(struct amded_file &file)
{
    if (file.fh->readOnly()) {
        std::cerr << PROJECT << ": File is read-only: "
                  << file.name << std::endl;
        return;
    }

    if (file.multi_tag) {
        strip_multitag(file);
        return;
    }

    TagLib::PropertyMap pm = file.fh->properties();
    pm.clear();
    file.fh->setProperties(pm);
    if (!(file.fh->save()))
        std::cerr << PROJECT << ": Failed to save file `"
                  << file.name
                  << "'"
                  << std::endl;
}
