#!/bin/sh
#
# h-craft championship startup script
#

# Function to find the real directory a program resides in.
# Feb. 17, 2000 - Sam Lantinga, Loki Entertainment Software
FindPath()
{
    fullpath="`echo $1 | grep /`"
    if [ "$fullpath" = "" ]; then
        oIFS="$IFS"
        IFS=:
        for path in $PATH
        do if [ -x "$path/$1" ]; then
               if [ "$path" = "" ]; then
                   path="."
               fi
               fullpath="$path/$1"
               break
           fi
        done
        IFS="$oIFS"
    fi
    if [ "$fullpath" = "" ]; then
        fullpath="$1"
    fi

    # Is the sed/ls magic portable?
    if [ -L "$fullpath" ]; then
        fullpath=`ls -l "$fullpath" |sed -e 's/.* -> //' |sed -e 's/\*//'`
    fi
    dirname $fullpath
}

# Set the home if not already set.
if [ "${HCRAFT_APP_PATH}" = "" ]; then
    HCRAFT_APP_PATH="`FindPath $0`"
fi
	
if [ -x "${HCRAFT_APP_PATH}/hcraft_bin" ]
then
	cd "${HCRAFT_APP_PATH}/"
	exec "./hcraft_bin" $*
elif [ -x "${HCRAFT_APP_PATH}/hovercraft64" ]
then
	cd "${HCRAFT_APP_PATH}/"
	exec "./hovercraft64" $*
fi

echo "Sorry, could not run H-Craft Championship."
echo "Maybe the executable flag is not set for the file 'hcraft_bin'." 
echo "Your file-browser might allow changing that flag."
echo "Alternatively cd into the game folder in bash and type: chmod +x hcraft_bin"
echo "If it still doesn't start try to cd into the game folder first and start it from there."

exit 1


