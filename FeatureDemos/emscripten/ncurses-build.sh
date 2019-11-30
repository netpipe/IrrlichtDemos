#!/bin/bash

#src_env.sh

#make original first to get the make_hash and make_keys files
./configure
make


cd /ncurses/ncurses
tar -xzf make_hash make_keys report_offsets tmp.tar


#grep and delete these lines from makefile
#make_keys$(BUILD_EXEEXT) : \ grep  then 17 lines delete
#$(BUILD_CC) -o $@ $(BUILD_CPPFLAGS) $(BUILD_CCFLAGS) $(tinfo)/make_keys.c $(BUILD_LDFLAGS) $(BUILD_LIBS)
#$(BUILD_CC) -o $@ $(BUILD_CPPFLAGS) $(BUILD_CCFLAGS) $(tinfo)/make_hash.c $(BUILD_LDFLAGS) $(BUILD_LIBS)
	#$(BUILD_CC) -o $@ $(BUILD_CPPFLAGS) $(BUILD_CCFLAGS) $(srcdir)/report_offsets.c $(BUILD_LDFLAGS) $(BUILD_LIBS)


emconfigure ./configure
make

build yeilds libncurses.a and libncurses++.a

