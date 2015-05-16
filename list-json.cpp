/*
 * Copyright (c) 2015 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file list-json.cpp
 * @brief Tag reader frontend for machines via JSON.
 */

#include <iostream>
#include <memory>
#include <json/json.h>
#include <json/writer.h>

#include "list-json.h"

static Json::Value data;

void
amded_push_json(const struct amded_file &file __attribute__((unused)))
{
    data["stuff"] = "is in here";
}

void
amded_list_json(void)
{
    Json::StreamWriterBuilder builder;
    /* This makes the JSON output minimally packed. */
    builder.settings_["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(data, &std::cout);
}
