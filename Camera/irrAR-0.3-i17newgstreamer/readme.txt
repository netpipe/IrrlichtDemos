Creator:
Michael Bok
www.nighsoft.com

License:
zlib

WARNING:
This library depends on ARToolKit which is under the GPL license, which will force your projects to be GPL as well (unless you buy ARToolkit's commercial license).

Notes:
This has only been tested on a 64bit linux OS (gentoo), it should and will work on windows and macs.

To Test (Linux):
Type make in this folder and run ./test

To Test (Windows):
Fix WDM_camera_flipV.xml as explained below and run test_irrAR.exe (exe may not be up to the current version of irrAR).

To Use:
Add irrAR.cpp and irrAR.h to your irrlicht project and refer to test.cpp for a simple usage guide.

ARToolKit Tips:
When on windows you will need to manually fix WDM_camera_flipV.xml for your camera. Look for the line - <camera show_format_dialog="true" friendly_name="PGR"> and then set friendly_name to your camera's specific name. 
...
Your camera's specific name can be found easily with the aid of a program called graph edit. After you find and download Graph Edit (which is free by microsoft), goto the top Graph->Insert Filters (or hit ctrl+F). This will bring up a window with a ton of selections, in most cases you will expand the choice "WDM Streaming Capture Devices". Under this selection you will see all the choices you have, in my case there is "Logitech QuickCam Orbit/Sphere". I would then take this exactly and put it into the WDM_camera_flipV.xml as explained above. This procedure will become simplified with later versions of irrAR.
...
Simply print out any of the patterns you choose to use from the ardata folder (the *.pdf's), the example test.cpp uses the hiro and sample1 patterns.

Requirements:
Irrlicht and ARToolkit

Other Documentation and Media:
Version 0.3 - http://www.youtube.com/watch?v=bWmGEpFxQb4
Version 0.1 - http://www.youtube.com/watch?v=6jKCFBVLFxE
Homepage - http://www.nighsoft.com/index.php?game=libs&page=irrar

Developed by Nighsoft at www.nighsoft.com