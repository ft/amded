#!/bin/zsh
# This is an example of how to process the data that taggit emits in
# its list-machine-readable mode (using the -m option).
# The code is written in zsh <http://zsh.sourceforge.net>.

emulate zsh
typeset -a taggit tags info

# Taggit uses ASCII end-of-transmission (EOT) bytes to seperate information
# from different files. So First, break up the input at EOT bytes.
# That would be hex 0x04 - and zsh makes that fairly simple (this assumes
# taggit to be located in the processes $PATH and it will process files from
# the current working directory matching the pattern '*.mp3'):
taggit=( ${(ps:\x04:)"$(taggit -m *.mp3)"} )

# Now iterate over the records for each file:
for fi in $taggit; do

    # Just to show that $taggit is an array that contains records for
    # each file processed by taggit, let's put in a marker line:
    print -- -------------------------------------------------------------------

    # Now each file contains a number of tags and each tag=value pair
    # is seperated from each other by ASCII end-of-text (ETX) bytes.
    # Again we use the same technique as above to split that up:
    tags=( ${(ps:\x03:)fi} )

    # And again, we start iterating over what we've just gathered:
    for ti in $tags; do

        # At this point, we got the tag=value pairs in $ti. The
        # tag names are seperated from their value data by ASCII
        # start-of-text (STX) bytes. So, we're once again using
        # zsh's parameter expansion split flag to gain easy access
        # to the actual data:
        info=( ${(ps:\x02:)ti} )
        printf '%12s | %s\n' ${info[1]} ${info[2]}
    done
done

exit 0
