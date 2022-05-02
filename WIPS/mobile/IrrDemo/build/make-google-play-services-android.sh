# Get google-play-services working. Used for admob and in-game billing.

if [ -z "$ANDROID_HOME" ]
then 
	echo "ANDROID_HOME must be set to your local Android-sd path (like for example /myhome/android/adt-bundle-linux-x86_64-20131030/sdk)"
	exit 1
fi
if [ -z "$HCRAFT_HOME" ]
then 
	export HCRAFT_HOME=$(pwd)/..
fi

# Copy the google-play-services lib 
cd $HCRAFT_HOME/build/android/libs
cp -R $ANDROID_HOME/extras/google/google_play_services/libproject/google-play-services_lib .
# something to update project files - not sure what that step does exactly, just followed documentation there
cd $ANDROID_HOME/tools
android update project --target android-22 --name hcraft1 --path $HCRAFT_HOME/build/android --library $HCRAFT_HOME/build/android/libs/google-play-services_lib
# We don't want a new (and also not absolute!) path in our project.properties. 
cd $HCRAFT_HOME
#hg revert ./android/project.properties
# Ensure to use android-13 as target
cd $HCRAFT_HOME/build/android/libs/google-play-services_lib 
android update project --path . --target android-10 --subprojects
# Copy android-support-v4.jar (probably not needed when only Android >= 4.x is targeted). 
cp $ANDROID_HOME/extras/android/support/v4/android-support-v4.jar $HCRAFT_HOME/build/android/libs
