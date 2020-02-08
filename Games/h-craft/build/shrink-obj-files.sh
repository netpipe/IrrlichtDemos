# will only run on linux until mingw decides to use a newer sed.
# all .lmo and all .obj files do always have 6 numbers after the point
# therefore we replace two typical unneeded patterns. ".000000" and ".500000"

export TOOL_FOLDER=`pwd`
if test `basename "$TOOL_FOLDER"` != build
then
    echo must be started from build folder
    exit -1
fi


find ${TOOL_FOLDER}/../media/levels -name "*.lmo" -exec sed -i s/.000000//g {} \;
find ${TOOL_FOLDER}/../media/levels -name "*.obj" -exec sed -i s/.000000//g {} \;
find ${TOOL_FOLDER}/../media/levels -name "*.lmo" -exec sed -i s/.500000/.5/g {} \;
find ${TOOL_FOLDER}/../media/levels -name "*.obj" -exec sed -i s/.500000/.5/g {} \;
