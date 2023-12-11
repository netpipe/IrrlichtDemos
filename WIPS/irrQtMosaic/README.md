Mosaicked
=====================
A [Photomosaic](http://en.wikipedia.org/wiki/Photographic_mosaic) generator written in C++ using Qt. 

Given base image and directory of sub-images, it constructs an image using only sub-images such a way that it resembles the base image.

How To Compile
------------------
open .pro file in QtCreator to compile.

Dependency
------------------
Requires Qt 4.x or higher.


Usage
------------------
Select a base picture to be used to construct the photomosaic image.

Select a directory where sub-images (mosaics) located

Enter the width (in pixels) of the photomosaic image.

Enter the height (in pixels) of the photomosaic image.

Enter the ratio of mosaics (all the images in the folder will be converted to this ratio, so make sure you have backups if you don't want your pictures to be altered.)

Select number of mosaics the photomosaic image contains, the higher this number is more precise the image will look. (This is an approximation, program will automatically round it up to the next possible number based on the specified ratio)

Enter precision of the photomosaic image (This is an experimental feature, the higher the number is more precise the output should be, given that you have large set of mosaics)

**Sample results can be downloaded from [here](http://goo.gl/0JBA3a)**


![Example photo mosaic](https://dl.dropbox.com/s/m9j08lwgxt9d55s/GUI.jpg)
![Example photo mosaic](https://dl.dropbox.com/s/zcfiyc6julqty65/Star%20Wars%20Mosaic%20Example.jpg)



  
