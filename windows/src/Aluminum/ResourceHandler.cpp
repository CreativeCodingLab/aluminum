#include "Aluminum/ResourceHandler.hpp"


namespace aluminum {
	
	
	ResourceHandler::ResourceHandler() { }

	std::string ResourceHandler::contentsOfFile(std::string &filename) {
		std::cout << filename.c_str() << " " << filename << std::endl;
		std::ifstream file;
		file.open(filename.c_str());

		if (!file) {
			printf("in ResourceHander::contentsOfFile : error, couldn't find file!\n");
			exit(0);
		}

		std::stringstream stream;
		stream << file.rdbuf();
		file.close();

		return stream.str();
	}
	
	string ResourceHandler::pathToResource(const std::string &resource) { return "implement me!"; }

	string ResourceHandler::pathToResource(const std::string &resource, const std::string &type) {
		string result = resource + "." + type; 
		std::cout << result << std::endl;
		return result;
	}
	
	void ResourceHandler::loadProgram(Program &p, const std::string &name, int pLoc, int nLoc, int tcLoc, int cLoc) {
	
		p.create();

		//string sv = pathToResource(name, "vsh");  // uncomment when it works
		string sv = name + ".vsh";
		p.attach(contentsOfFile(sv), GL_VERTEX_SHADER);

		if (pLoc >= 0) glBindAttribLocation(p.id(), pLoc, "vertexPosition");
		if (nLoc >= 0) glBindAttribLocation(p.id(), nLoc, "vertexNormal");
		if (tcLoc >= 0) glBindAttribLocation(p.id(), tcLoc, "vertexTexCoord");
		if (cLoc >= 0) glBindAttribLocation(p.id(), cLoc, "vertexColor");

		//string sp = pathToResource(name, "fsh");  // uncomment when it works
		string sp = name +".fsh";
		p.attach(contentsOfFile(sp), GL_FRAGMENT_SHADER);
		p.link();
	}
	
	void ResourceHandler::loadTexture(Texture &t, const std::string &name) {

		FreeImage_Initialise();  // Need this call to ensure FreeImage works...

		const char *filename = name.c_str();
		FREE_IMAGE_FORMAT format = FIF_UNKNOWN;
		FIBITMAP *dib(0);
		BYTE *bits(0);
		unsigned int width(0), height(0), pixSz(0);
		FREE_IMAGE_COLOR_TYPE colorf;
		FREE_IMAGE_TYPE typef;
		GLenum pixFormat = GL_RGBA;

		//check the file signature and deduce its format
		format = FreeImage_GetFileType(filename, 0);
		//if still unknown, try to guess the file format from the file extension
		if (format == FIF_UNKNOWN)
			format = FreeImage_GetFIFFromFilename(filename);
		//if still unkown, return failure
		if (format == FIF_UNKNOWN) {
			printf("Unable to load file, exiting..");
			exit(1);
		}

		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(format))
			dib = FreeImage_Load(format, filename);
		//if the image failed to load, return failure
		if (!dib) {
			printf("Image failed to load, exiting..");
			exit(1);
		}


		bits = FreeImage_GetBits(dib);
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		typef = FreeImage_GetImageType(dib);
		colorf = FreeImage_GetColorType(dib);
		pixSz = FreeImage_GetBPP(dib);

		switch (colorf) {
		case FIC_MINISBLACK:
			//right now this is a special case for different types of FONT textures...
			//converting single channel to rgba so shaders are the same...
			//prob should be in its own font loader class, not a general texture loading method
			printf("FIC_MINISBLACK\n");
			pixFormat = GL_RED;
			//dib = FreeImage_ConvertTo32Bits(dib);
			//pixFormat = GL_RGBA;
			break;
		case FIC_MINISWHITE:
			printf("FIC_MINISWHITE\n");
			break;
		case FIC_PALETTE:
			printf("FIC_PALETTE\n");
			break;
		case FIC_CMYK:
			printf("FIC_CMYK\n");
			break;
		case FIC_RGB:
			printf("FIC_RGB\n");
			pixFormat = GL_RGB;
			//dib = FreeImage_ConvertTo32Bits(dib);
			//pixFormat = GL_RGB;
			break;
		case FIC_RGBALPHA:
			printf("FIC_RGBA\n");
			//pixFormat = GL_RGBA;
			break;
		default:
			printf("format %d not handled!\n", colorf);
			break;
		}

		//probably want to have a single channel format plus a single+alpha

		printf("format = %d\n", format);
		printf("Image type = %d\n", typef);
		printf("w/h = %d/%d\n", width, height);
		printf("pixel bit size: %d\n", pixSz);
		printf("FreeImage_IsTransparent = %d\n", FreeImage_IsTransparent(dib));
		
		GLubyte* data = (GLubyte*)malloc((width * height * 3));
		memcpy((GLubyte*)data, (GLubyte*)bits, width * height * 3);

		t = Texture(data, width, height, GL_RGBA, pixFormat, GL_UNSIGNED_BYTE);
		FreeImage_Unload(dib);

	}
		
}
