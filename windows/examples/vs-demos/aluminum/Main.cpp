#include "BasicColor.cpp"
#include "Lighting.cpp"
#include "Texturing.cpp"
#include "Billboarding.cpp"
#include "MiniPlanet.cpp"
#include "Transformation.cpp"
#include "Cellular.cpp"
#include "ModelFromObjFile.cpp"
#include "VertexDisplacement.cpp"
#include "FBOExample.cpp"
#include "NoiseSphere.cpp"
#include "WBOIT.cpp"
#include "ImageProcessing.cpp"
#include "SkyBox.cpp"


int main() {
	//Basic().start("Aluminum: Week1 BasicColor", 400, 300);
    //Transformation().start("Aluminum: Week2a Transformation", 400, 300);
	//TextureExample().start("Aluminum: Week2b Texturing", 400, 300);
    //Lighting().start("Aluminum: Week3a Lighting", 400, 300);
    //FBOExample().start("Aluminum: Week3b FBOExample", 400, 300);
    //Cellular().start("Aluminum: Week3c Cellular", 400, 300);
    //VertexDisplacement().start("Aluminum: Week4a VertexDisplacement", 400, 300);  // <-- Less Buggy!!
    //ModelFromObjFile().start("Aluminum: Week4b ModelFromObjFile", 400, 300);
    NoiseSphere().start("Aluminum: Week5a NoiseSphere", 400, 300);
    //SkyBox().start("Aluminum: Week5b SkyBox", 400, 300); // <-- Buggy!!
    //MiniPlanet().start("Aluminum: Week5c MiniPlanet", 400, 300); // <-- Buggy!!
    //WBOIT().start("Aluminum: Week5d WBOIT", 400, 300); 
    //ImageProcessing().start("Aluminum: Week6a ImageProcessing", 400, 300); // <-- Buggy!!
    //Billboarding().start("Aluminum: Week6b Billboarding", 400, 300); // <-- less Buggy!!

    return 0;
}