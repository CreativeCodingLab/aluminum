aluminum
========

Lightweight, minimal cross platform OpenGL framework using C++11 (or Objective-C++11 on OSX/iOS). Supports OpenGL 3.2 on osx/cocoa, windows/freeglut, and linux/freeglut. Supports OpenGL ES 2.0 on iOS.

***

Aluminum was initially created for the Allosphere Research Facility at UC Santa Barbara and is based on the allocore library (now called AlloSystem), written by Graham Wakefield, Lance Putnam, and others. (https://github.com/AlloSphere-Research-Group/AlloSystem). 

*** 

Required libraries: 
    glm@0.9.5.1 (OSX/iOS, Windows, Linux), 
    freeglut@2.8.1 (Windows, Linux), 
    freeimage@3.16.0 (Windows, Linux), 
    glew@1.10.0+ (Windows),
    ~~assimp3~~

*** 

Features: active, passive, and anaglyph stereo rendering; signed-distance fonts; textured fonts; high resolution timing/animation methods (via C++11's chrono); model loading (via assimp3); texture reading/writing (via freeimage); 6DOF vector camera class; etc. 

Sample OSX/Cocoa projects using XCode 4.6.2 provided.

Sample iOS projects using XCode 4.6.2 provided.

Sample Windows project using Visual Studio 2013 provided. 

Linux requires freeglut2.8.1 (or above?), see readme in linux directory. (may need a little work...)

***

Issues - In OSX the MacPorts installation of FreeImage (installed into /opt/local/include/FreeImage.h) has some kind of type clash. Resolve by replacing all instances of BOOL in the FreeImage.h file with a unique name, like BOOL_FI. (In this header, BOOL is a tyepdef for int32, not sure what exactly the issue is, but easy enough to fix).


