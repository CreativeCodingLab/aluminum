#include "Aluminum/FreeGlutGLView.hpp"
#include "Aluminum/RendererWin32.hpp"

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);
using std::cerr;
using std::cout;
using std::endl;

namespace aluminum {
    int width;
    int height;
	int time, timebase = 0;
    RendererWin32* renderer;
    
	//static std::time_t lastTime = std::time(nullptr);
	static std::clock_t lastTime = std::clock();

    FreeGlutGLView::FreeGlutGLView() {}
	/*
	FreeGlutGLView::~FreeGlutGLView() {
		FreeImage_DeInitialise();
	}
	*/
    void reshape(GLint _w, GLint _h) {
	    width = _w;
	    height = _h;

	    renderer->width = width;
	    renderer->height = height;

	    glViewport(0,0,width,height);

	    printf("in reshape: %d %d\n", width, height);
	    GLuint bbb;
	    //glGenVertexArrays(1, &bbb);
    }

    void display() {

	    //printf("in FreeGlutGLView : display()\n");
		renderer->tick();
	    renderer->onFrame();
		glFlush();
		glutSwapBuffers();
    }

    void animate() {
		renderer->frameCount++;
		time = glutGet(GLUT_ELAPSED_TIME);
		if (time - timebase > 1000) {
			printf("FPS:%4.2f\n",
				renderer->frameCount*1000.0 / (time - timebase));
			timebase = time;
			renderer->frameCount = 0;
		}
		//renderer->tick();
	 //   float dt;
  //      //std::time_t now = std::time(nullptr);
		//static std::clock_t now = std::clock();
		//dt = float(now - lastTime);
	 //   lastTime = now;
		//printf("animate: lastTime %i ", now);
	    glutPostRedisplay();
    }

    void pressed(int button, int state, int x, int y ) {
    	printf("button : %d %d %d %d\n", button, state, x, y);
	    //check state to see if sending down or up...
		if (state == 0)
		{
			renderer->mouseDown(x, y);
		}
		else if (state == 1)
		{
			renderer->mouseUp(x, y);
		}
		
		
    }
    void dragged(int x, int y ) {
    //	printf("motion : %d %d\n", x, y);
	    renderer->mouseDragged(x,y);
    }
    void moved(int x, int y ) {
    	printf("motion : %d %d\n", x, y);
	    renderer->mouseMoved(x,y);
    }


    void keyboard(unsigned char key, int x, int y) {
		renderer->keyboard(key, x, y);
		
		switch(key) {
		    case 27: 
			    exit(0);
			    break;
		    case '1':
			    printf("you pressed the number 1!\n");
			    glutFullScreen();
			    break;
		    case '2':
			    glutReshapeWindow(400,400);
			    break;
	    }

	    printf("done pressing...\n");
    }

    FreeGlutGLView* FreeGlutGLView::start(void* _renderer) {
	    return FreeGlutGLView::start(_renderer, "allomin");
    }

	FreeGlutGLView* FreeGlutGLView::start(void* _renderer, std::string name) {
		return FreeGlutGLView::start(_renderer, "allomin", 200, 200);
	}

    FreeGlutGLView* FreeGlutGLView::start(void* _renderer, std::string name, int _width, int _height) {
		printf("freglutglview start");
	    renderer = (RendererWin32*) _renderer;

	    /* annoying useless setup for glutInit */

		char* argv[] = {_strdup(name.c_str())};
	    int argc = 1;
	
	    glutInit(&argc, argv);
	    glutInitContextVersion(3,3);
		
		printf("freglutglview start");
		glutInitWindowSize(_width, _height);
	    glutCreateWindow(name.c_str());

		//glew initialization
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)	{
			cerr << "Error: " << glewGetErrorString(err) << endl;
		}
		else {
			if (GLEW_VERSION_3_3) {
				cout << "Driver supports OpenGL 3.3\nDetails:" << endl;
			}
		}
		err = glGetError(); //this is to ignore INVALID ENUM error 1282
	//	GL_CHECK_ERRORS


			//output hardware information
		cout << "\tUsing GLEW " << glewGetString(GLEW_VERSION) << endl;
		cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
		cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
		cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
		cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

		// call this ONLY when linking with FreeImage as a static library
		
		#ifdef FREEIMAGE_LIB
				FreeImage_Initialise();
				cout << "\tFreeImage: " << FreeImage_GetVersion() << endl;
		#endif
		

    //	glutGameModeString("1280x1024:32@60");
    //	glutEnterGameMode();
		
		renderer->setStartTick();
	    renderer->onCreate();

	    glutDisplayFunc(&display);
	    glutReshapeFunc(&reshape);
		glutKeyboardFunc(&keyboard);
	    glutMouseFunc(&pressed);
	    glutMotionFunc(&dragged);
	    glutPassiveMotionFunc(&moved);
		glutIdleFunc(&animate);
		printf("freglutglview start");
		//lastTime = std::clock();
		//std::time(&lastTime);
	    //gettimeofday(&lastTime, NULL);
		printf("freglutglview start");
	    glutMainLoop();

	    //glutInitDisplayMode(GL_RGBA);
	    //glutInitWindowSize(200,200);
	    //glutCreateWindow("test");
		printf("freglutglview start");
	    return NULL;
	    //return glView;
		
    }
}

