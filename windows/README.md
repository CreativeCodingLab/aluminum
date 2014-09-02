Building Aluminum Library on Visual Studio 2013
===============================================

###Required Libraries:
+ [FreeImage](http://freeimage.sourceforge.net/index.html) You will need to compile the Debug configuration in order to use the library, which can be a bit involved...OR you can [email](kalmry2@uic.edu) me and I can send you a precompiled version Ive generated.

+ [freeglut](http://freeglut.sourceforge.net/ )
Use the Martin Payne's [prepackaged release](http://files.transmissionzero.co.uk/software/development/GLUT/freeglut-MSVC.zip), unless you want to compile from source

+ [GLM](http://glm.g-truc.net/0.9.5/index.html): You can download the Headers [here](http://sourceforge.net/projects/ogl-math/files/latest/download?source=files)

+ [GLEW](http://glew.sourceforge.net/) The windows binaries can be downloaded [here](https://sourceforge.net/projects/glew/files/glew/1.11.0/glew-1.11.0-win32.zip/download)
>When installing glew, there are a few extra steps you have to perform in order to ensure everything works correctly. 
>Depending on your system's architecutre, you will need to copy the library to one of the following directories:

>>- glew\bin\Release\Win32\glew32.dll **to** %SystemRoot%/system32

>>- glew\bin\Release\Win32\glew32.dll **to** %SystemRoot%/SysWOW64

>>> I recognize that I am refering to the same glew32.dll file for both architecutes. There is a glew32.dll under the 64bit architecture, however this appears to be a bug and VS (at least on my system) refuses to acknowledge the 64bsit file. So dont use it. Should you encounter any problems do not hesistate to [ask](kalmry2@uic.edu)\. 


***
### Configuring VS2013 for use with Aluminum

The VS2013 project utilizes Global variables to allow for maximum flexibility on the developer's part (**YOU**). Ive configured the project such that it assumes you have the following global variables defined on your system.

>>- **ALUMINUM_PATH** e.g. defines the base location of the aluminum library, ie, C:\aluminum
- **GLM_PATH**
- **GLEW_PATH** 
- **FREEIMAGE_PATH**
- **FREEGLUT_PATH**


To define a global variable go to:
>>- *Control Panel\System and Security\System*
- Select "Advanced System Settings", the "System Properties" window will appear,
- Select "Environmental Variables"
- Define youre variables!



###Hints:
**%SystemRoot%** == "**c:\Windows**"
