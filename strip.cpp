/*
 * Copyright (c) 2013-2017 amded workers, All rights reserved.
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
#include "setup.h"
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
    const unsigned int unsupported = pm.unsupportedData().size();
    pm.clear();
    file.fh->setProperties(pm);

    if (!get_opt(AMDED_KEEP_UNSUPPORTED_TAGS) && unsupported > 0)
        file.fh->removeUnsupportedProperties(pm.unsupportedData());

    if (!(file.fh->save()))
        std::cerr << PROJECT << ": Failed to save file `"
                  << file.name
                  << "'"
                  << std::endl;
}
