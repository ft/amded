/*
 * Copyright (c) 2013-2019 amded workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file amded-vendor.h
 * @brief Vendor specific information
 *
 * This file is *only* included, if the VENDOR_BUILD macro is set in the build
 * process. That can be achieved be calling make like this:
 *
 * @code
 * % make ADDTOCXXFLAGS=-DVENDOR_BUILD
 * @endcode
 *
 * If you want to call the ‘depend’ target as well, then it makes sense to pass
 * that option in with that, too:
 *
 * @code
 * % make depend ADDTOCXXFLAGS=-DVENDOR_BUILD
 * @endcode
 */

#ifndef INC_AMDED_VENDOR_H
#define INC_AMDED_VENDOR_H

/**
 * Vendor-specific version information.
 *
 * If set to "foo-bar", amded's version information output will look like
 * this:
 *
 * @code
 * amded version 0.5 (foo-bar)
 * @endcode
 */
#define VENDOR_VERSION "foo-OS package version 0.5-9"

#endif /* INC_AMDED_VENDOR_H */
