Creator:
Michael Bok
www.nighsoft.com

License:
zlib

Notes:
This has only been tested on a 64bit linux OS (gentoo) and windows with dev-cpp. Should work on Macs. This release comes with CvIrrAviTexture and CvIrrCamTexture.

Requirements:
Irrlicht and OpenCV

To Test (Linux):
type make in this folder and run ./test

To Test (Windows):
Compile test.cpp as directed below.

To Calibrate Your Camera:
You will need the camera matrix and lens distortion vector for your camera, and place it in the test.cpp appropriately. A good tool for getting your camera's matrix and lens distortion is at http://www.ics.forth.gr/~xmpalt/research/camcalib_wiz/index.html . It only works on windows, (it might wine?)

To Use:
add CvIrrCamController.cpp and CvIrrCamController.h to your irrlicht project and refer to test.cpp for a simple usage guide.

Other Documentation and Media:
Youtube video of demo in action: 
Version 0.1 - http://www.youtube.com/watch?v=nciqfiKD8_g
Version 0.3.8 - http://www.youtube.com/watch?v=ILwTAG8XbmA
Writer of software: www.nighsoft.com