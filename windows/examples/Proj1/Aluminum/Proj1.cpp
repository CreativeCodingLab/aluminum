#include "Aluminum/Includes.hpp"
#include "Aluminum/FreeGlutGLView.hpp"
#include "Aluminum/RendererWin32.hpp"
#include "Aluminum/Program.hpp"
#include "Aluminum/MeshBuffer.hpp"
#include "Aluminum/MeshData.hpp"
#include "Aluminum/Shapes.hpp"
#include "Aluminum/Camera.hpp"
#include "Aluminum/Utils.hpp"
#include "Aluminum/MeshUtils.hpp"
#include "Aluminum/ResourceHandler.hpp"
#include "Aluminum/FBO.hpp"
#include "Aluminum/Behavior.hpp"
#include <chrono>
#include <time.h>

using namespace aluminum;

class Proj1 : public RendererWin32 {
public:

	void onCreate(){}

	void onFrame(){} 

};



int main(){
	Proj1().start();
	return 0;
}
