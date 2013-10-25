#include <map>

#include "setup.h"
#include "taggit.h"
#include "value.h"

static std::map< enum tag_id, Value > newtags;

void
add_tag(enum tag_id id, Value v)
{
    newtags[id] = v;
}
