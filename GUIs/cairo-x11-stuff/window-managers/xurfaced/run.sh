#!/bin/sh

TTY="vt5"

if [ -f xurfaced ]
then

    echo "Starting xurfaced on tty $TTY"
    xinit ./xurfaced -display :0 -- :0 $TTY

else

    echo "xurfaced file does not exist. Forgot to compile?"

fi
