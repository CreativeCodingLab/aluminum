#pragma once
#include "Aluminum/Includes.hpp"
#include "Aluminum/Texture.hpp"
#include "Aluminum/Program.hpp"

namespace aluminum {

	class ResourceHandler {

	public:
		ResourceHandler();

		//const char *contentsOfFile(std::string &file);
		std::string contentsOfFile(std::string &file);
		std::string pathToResource(const std::string &resource);
		std::string pathToResource(const std::string &resource, const std::string &type);
		void loadProgram(Program &p, const std::string &name, int pLoc, int nLoc, int tcLoc, int cLoc);
		void loadTexture(Texture &t, const std::string &name);
	};

}

