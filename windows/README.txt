Building Aluminum Library on Visual Studio 2013

Required Libraries:

GLM - http://glm.g-truc.net/0.9.5/index.html 
	  You can download the Headers here http://sourceforge.net/projects/ogl-math/files/latest/download?source=files

glew - http://glew.sourceforge.net/
	   The windows binaries can be downloaded here https://sourceforge.net/projects/glew/files/glew/1.11.0/glew-1.11.0-win32.zip/download
	  When installing glew, there are a few extra steps you have to perform in order to ensure everything works correctly. 
		Copy the libraries to the following directories:
			bin/glew32.dll	    to    	%SystemRoot%/system32
			lib/glew32.lib	    to    	{VC Root}/Lib
			include/GL/glew.h	    to    	{VC Root}/Include/GL
			include/GL/wglew.h	    to    	{VC Root}/Include/GL
		Hints:
			%SystemRoot% == "C:\Windows"
				Curious what other environmental variables Windows is hiding from you? Type "set"
			{VC Root} or the Visual C++ root directory, typically "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC"


	  
FreeImage - http://freeimage.sourceforge.net/index.html 
			You will need to compile the Debug configuration in order to use the library, which is a bit involved...
				OR
			You can email me and I can send you a precompiled version Ive generated. The zip file is pretty large as I havent cleaned it up. 
				However it should give you what you need

freeglut - http://freeglut.sourceforge.net/ 
		   Use the Martin Payne's prepackaged release, unless you want to compile from source.
		   http://files.transmissionzero.co.uk/software/development/GLUT/freeglut-MSVC.zip
		

The VS2013 project utilizes Global variables to allow for maximum flexibility on the developer's (YOU) part. Since everyone keeps their libraries 
in different places, this seemed to be the simplest solution. 
They are defined as follows: 
	{ALUMINUM, GLM, GLEW, FREEIMAGE, FREEGLUT}_PATH

	The ALUMINUM_PATH defines the base location of the aluminum library, ie, C:\aluminum
	
	
	