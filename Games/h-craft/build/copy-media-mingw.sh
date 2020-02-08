#!/bin/sh
# Copy the media from their original location into the working folder
# You keep original media for releases, while the copied media can be worked with.

export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi

if test -z "$MEDIA_PATH"
then
    export MEDIA_PATH=${TOOL_FOLDER}/../../media_hc1
fi

rm -rf ${TOOL_FOLDER}/../media
cp -r ${MEDIA_PATH}/media $TOOL_FOLDER/..
