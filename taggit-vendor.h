/*
 * Copyright (c) 2013 taggit workers, All rights reserved.
 * Terms for redistribution and use can be found in LICENCE.
 */

/**
 * @file taggit-vendor.h
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

#ifndef INC_TAGGIT_VENDOR_H
#define INC_TAGGIT_VENDOR_H

/**
 * Vendor-specific version information.
 *
 * If set to "foo-bar", taggit's version information output will look like
 * this:
 *
 * @code
 * taggit version 0.5 (foo-bar)
 * @endcode
 */
#define VENDOR_VERSION "foo-OS package version 0.5-9"

#endif /* INC_TAGGIT_VENDOR_H */
