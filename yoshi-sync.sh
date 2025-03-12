#! /bin/bash

# To use this script you must create a script called config-yoshi-sync.sh
# which defines two variables: YOSHI_USER and YOSHI_DESTINATION.
# YOSHI_USER is your username on the Yoshi server, and YOSHI_DESTINATION
# is the directory where you want to sync to on the Yoshi server.
#
# Example script (ignoring leading '#'s):
# #! /bin/bash
# YOSHI_USER=johndoe
# YOSHI_DESTINATION=/project/johndoe/LED-Video-Wall

YOSHI_CONFIG=config-yoshi-sync.sh
if [ -f $YOSHI_CONFIG ]; then
    source $YOSHI_CONFIG
else
    echo "Error" $YOSHI_CONFIG "not found. Please look at the comments" \
         "in this script and configure appropriately."
    exit 1
fi

rsync \
    -av \
    --exclude */obj \
    --exclude */led-wall-server \
    ./server ./protocol \
    $YOSHI_USER@yoshi.cse.buffalo.edu:$YOSHI_DESTINATION
