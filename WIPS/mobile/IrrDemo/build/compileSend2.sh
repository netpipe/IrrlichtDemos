#!/bin/bash

#use line below to update the project to a new platform
#/home/Dev/android-sdk-linux/tools/android update project --path . --target android-19

export ANDROID_HOME=/home/Dev/android-sdk-linux
export PATH=:/home/Dev/android-sdk-linux/tools/lib:/home/Dev/android-sdk-linux/tools:/home/Dev/android-sdk-linux/platform-tools:/home/Dev/android-ndk-r10e

export ANDROID_NDK=/home/Dev/android-ndk-r10e
export PATH=.:/home/micha/bin:/sbin:/bin:/usr/sbin:/usr/bin:/usr/X11R6/bin:/usr/local/sbin:/usr/local/bin:/usr/games:/:$ANDROID_HOME/tools:$ANDROID_HOME/platform-tools:/home/Dev/android-ndk-r10e


ndk-build && ant debug && adb install -r bin/NativeActivity-debug.apk && adb kill-server

#exit 0

