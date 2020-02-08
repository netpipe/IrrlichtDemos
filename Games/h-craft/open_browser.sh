#!/bin/sh
# Try to find a browser and start it with the parameters given to this script
# Terms of the GNU General Public License apply

HOMEPAGE=$@
BROWSER=`which htmlview`

if [ ! -x "$BROWSER" ]; then
	
	BROWSER=`which firefox`
	if [ ! -x "$BROWSER" ]; then
		
		BROWSER=`which opera`
		if [ ! -x "$BROWSER" ]; then
	
			BROWSER=`which mozilla`
			if [ ! -x "$BROWSER" ]; then
	
				BROWSER=`which konqueror`
				if [ ! -x "$BROWSER" ]; then
	
					BROWSER=`which galeon`
					if [ ! -x "$BROWSER" ]; then

						BROWSER=`which iceweasel`
						if [ ! -x "$BROWSER" ]; then

							BROWSER=`which netscape`
							if [ ! -x "$BROWSER" ]; then
								echo Please start your favorite webbrowser and open:
								echo $@
								exit 1
							fi
						fi
					fi
				fi
			fi
		fi
	fi
fi

$BROWSER $HOMEPAGE &
