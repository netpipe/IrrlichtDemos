//
//  ResPath.h
//  Mac Example With Irrlicht
//
//  Created by Karl Kirch on 7/25/11.
//  Copyright 2011 MySelf. All rights reserved.
//

#ifndef Mac_Example_With_Irrlicht_ResPath_h
#define Mac_Example_With_Irrlicht_ResPath_h

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <iostream>

char* resPath(char* file );

char* resPath(char* file )
{
    // ----------------------------------------------------------------------------
#ifdef __APPLE__    
    
    // Getting the Resource folder inside the .app and append the file to it
    static char pathBase[PATH_MAX];
    char path[PATH_MAX];
    
    if(!strlen(pathBase)){
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
        if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)pathBase, PATH_MAX))
        {
            // error!
        }
        CFRelease(resourcesURL);
        strncat(pathBase,"/",PATH_MAX);
    }
    
    strcpy(path,pathBase);
    
    strncat(path,file,PATH_MAX);
    
    return strncat(path,"",PATH_MAX);
    
#else
    
    return file;
    
#endif
    // ----------------------------------------------------------------------------
}



#endif