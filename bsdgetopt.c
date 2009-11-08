/*    $NetBSD: getopt.c,v 1.27 2005/11/29 03:12:00 christos Exp $    */

/*
 * Copyright (c) 1987, 1993, 1994
 *    The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * getopt.c from NetBSD
 *     Traditional getopt() implementation.
 *     No permutations.
 *
 * Changes:
 *     - teared the file down to what we need
 *     - ripped out things we don't have
 *     - converted the function definition to ANSI style
 *     - using PROJECT macro instead of getprogname(),
 *       which is not portable.
 *     - casting nargv[optind] to (char *) to shut up
 *       compiler warning (when it gets assigned to optarg)
 *     - reindented using 4 spaces, as the rest of the code.
 *     - renaming getopt to bsd_getopt to avoid name problems.
 *
 *  -ft
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __UNCONST(a) ((void *)(unsigned long)(const void *)(a))

/** if error message should be printed */
int   opterr = 1;
/** index into parent argv vector */
int   optind = 1;
/** character checked for validity */
int   optopt;
/** reset getopt */
int   optreset;
/** argument associated with option */
char *optarg;

#define BADCH  (int)'?'
#define BADARG (int)':'
#define EMSG   ""

/**
 * Parse argc/argv argument vector.
 *
 * This is the traditional getopt() implementation as found in NetBSD.
 * No permutations.
 *
 * @param   nargc       number of arguments
 * @param   *nargv[]    array of argument strings
 * @param   *ostr       the options string
 *
 * @return      the processed option character casted to int.
 * @sideeffects sets the global variables opterr, optind, optopt,
 *              optreset and optarg.
 */
int
bsd_getopt(int nargc, const char *nargv[], const char *ostr)
{
    static const char *place = EMSG;    /* option letter processing */
    char *oli;                          /* option letter list index */

    if (optreset || *place == 0) {      /* update scanning pointer */
        optreset = 0;
        place = nargv[optind];
        if (optind >= nargc || *place++ != '-') {
            /* Argument is absent or is not an option */
            place = EMSG;
            return (-1);
        }
        optopt = *place++;
        if (optopt == '-' && *place == 0) {
            /* "--" => end of options */
            ++optind;
            place = EMSG;
            return (-1);
        }
        if (optopt == 0) {
            /*
             * Solitary '-', treat as a '-' option
             * if the program (eg su) is looking for it.
             */
            place = EMSG;
            if (strchr(ostr, '-') == NULL)
                return -1;
            optopt = '-';
        }
    } else
        optopt = *place++;

    /* See if option letter is one the caller wanted... */
    if (optopt == ':' || (oli = strchr(ostr, optopt)) == NULL) {
        if (*place == 0)
            ++optind;
        if (opterr && *ostr != ':')
            (void)fprintf(stderr,
                "%s: unknown option -- %c\n", PROJECT,
                optopt);
        return (BADCH);
    }

    /* Does this option need an argument? */
    if (oli[1] != ':') {
        /* don't need argument */
        optarg = NULL;
        if (*place == 0)
            ++optind;
    } else {
        /*
         * Option-argument is either the rest of this argument or the
         * entire next argument.
         */
        if (*place)
            optarg = __UNCONST(place);
        else if (nargc > ++optind)
            optarg = (char *)nargv[optind];
        else {
            /* option-argument absent */
            place = EMSG;
            if (*ostr == ':')
                return (BADARG);
            if (opterr)
                (void)fprintf(stderr,
                    "%s: option requires an argument -- %c\n",
                    PROJECT, optopt);
            return (BADCH);
        }
        place = EMSG;
        ++optind;
    }
    return (optopt);            /* return option letter */
}
