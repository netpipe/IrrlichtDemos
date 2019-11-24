========================================================================
 ConsoleViewer Project Overview
========================================================================

 Simple Demo of using NVSG with the X-Windows native Xlib SDK. 
 
 -----------------------------------------------------------------------
 
 To use the included Makefile export the following environment variables:

 Your compiler:
 $NVSG_CPP (e.g.: g++)

USAGE:

$./ConsoleViewer <file_to_open> <dir1> <dir2> <dir3> ...

Where: 
     file_to_open: Is a model that you want to display using ConsoleViewer
                   , e.g., scene.nvsg or scene.nbf (Any supported 3D format)

     dir[1,2,3]  : Are directories where required texture, effect and/or
                   volume-data files are present. 

     If the file_to_open belongs to media directory, then you can copy the 
     required texture, effect and/or volume-data files in appropriate 
     directories below media directory if the files are not already present
     there and skip specifying dir1, dir2, dir3, ...
  
     But, in case, file_to_open does not belong to media directory, it is 
     mandatory to specify all the texture, effect and/or volume directories
     on commandline, that are going to be used by model.	
     Any no. of directories (allowed by GCC) can be specified for textures, 
     effects and/or volume-data files.
 
     If no arguments provided, ConsoleViewer displays a default scene.


 Note: You will need to have X11 headers and libraries installed.
 
 Also you might need to edit the Makefile to specify your Xlib include/library
 paths and libs to link with. 
  

/////////////////////////////////////////////////////////////////////////////
