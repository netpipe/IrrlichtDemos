#/bin/bash
# Version 2 from 2007/07/18 changed:
# - search now also for libalut.so.0 and prefer it to libalut.so
# - search now also for libopenal.so.1 and libopenal.so.0 and prefer them to libopenal.so
# Version 3 also from 2007/07/18 changed:
# - do not overwrite the backups (ouch)

if [ $# = 0 ]
then
	echo "To use system sound libraries call:   $0 system"
	echo "To restore game sound libraries call: $0 game"
	exit 1
fi

case $1 in 
	system) 
		# check if system soundlibraries are installed and readable
		if ! test -r /usr/lib/libalut.so && ! test -r /usr/lib/libalut.so.0
		then
			echo "No libalut.so in /usr/lib or no read rights. Please install freealut."
			exit 1
		fi
		if ! test -r /usr/lib/libopenal.so && ! test -r /usr/lib/libopenal.so.1 && ! test -r /usr/lib/libopenal.so.0
		then
			echo "No libopenal.so in /usr/lib or no read rights. Please install openal."
			exit 1
		fi

		# backup game sound libraries if they are not yet backuped otherwise delete them
		if ! test -r libs/linux_shared/libalut.so.0.backup
		then
			mv -f libs/linux_shared/libalut.so.0 libs/linux_shared/libalut.so.0.backup
		else
			rm -f libs/linux_shared/libalut.so.0
		fi
		if ! test -r libs/linux_shared/libopenal.so.1.backup
		then
			mv -f libs/linux_shared/libopenal.so.1 libs/linux_shared/libopenal.so.1.backup
		else
			rm -f libs/linux_shared/libopenal.so.1
		fi
			
		# set links to system sound libraries
		if test -r /usr/lib/libalut.so.0
		then
			ln -s /usr/lib/libalut.so.0 libs/linux_shared/libalut.so.0 
		else
			if test -r /usr/lib/libalut.so
			then
				ln -s /usr/lib/libalut.so libs/linux_shared/libalut.so.0 
			fi
		fi
		if test -r /usr/lib/libopenal.so.1 
		then
			ln -s /usr/lib/libopenal.so.1 libs/linux_shared/libopenal.so.1 
		else
			if test -r /usr/lib/libopenal.so.0
			then
				ln -s /usr/lib/libopenal.so.0 libs/linux_shared/libopenal.so.1 
			else
				if test -r /usr/lib/libopenal.so
				then
					ln -s /usr/lib/libopenal.so libs/linux_shared/libopenal.so.1 
				fi
			fi
		fi
		;;
	game) 	# move backuped game sound libraries back to original names
		test -r libs/linux_shared/libalut.so.0.backup && mv -f libs/linux_shared/libalut.so.0.backup libs/linux_shared/libalut.so.0 
		test -r libs/linux_shared/libopenal.so.1.backup && mv -f libs/linux_shared/libopenal.so.1.backup libs/linux_shared/libopenal.so.1 
		;;
	*) 	echo "To use system sound libraries call:   $0 system"
		echo "To restore game sound libraries call: $0 game"
		exit 1;;
esac

exit 0
