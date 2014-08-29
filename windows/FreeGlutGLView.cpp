#include "FreeGlutGLView.hpp"
#include "RendererWin32.hpp"

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);
using std::cerr;
using std::cout;
using std::endl;

namespace aluminum {
    int width;
    int height;
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
	    renderer->onFrame();
	    glutSwapBuffers();
    }

    void animate() {
	    float dt;
        //std::time_t now = std::time(nullptr);
		static std::clock_t now = std::clock();
		dt = float(now - lastTime);
	    lastTime = now;

	    glutPostRedisplay();
    }

    void pressed(int button, int state, int x, int y ) {
    //	printf("button : %d %d %d %d\n", button, state, x, y);
	    //check state to see if sending down or up...
	    renderer->mouseDown(x,y);
    }
    void dragged(int x, int y ) {
    //	printf("motion : %d %d\n", x, y);
	    renderer->mouseDragged(x,y);
    }
    void moved(int x, int y ) {
    //	printf("motion : %d %d\n", x, y);
	    renderer->mouseMoved(x,y);
    }


    void keyboard(unsigned char key, int x, int y) {
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
		    default: 
			    printf("you pressed %c\n", key);
			    break;
	    }

	    printf("done pressing...\n");
    }

    FreeGlutGLView* FreeGlutGLView::start(void* _renderer) {
	    return FreeGlutGLView::start(_renderer, "allomin");
    }


    FreeGlutGLView* FreeGlutGLView::start(void* _renderer, std::string name) {

	    renderer = (RendererWin32*) _renderer;

	    /* annoying useless setup for glutInit */
	    char* argv[] = {_strdup(name.c_str())};
	    int argc = 1;

	    glutInit(&argc, argv);
	    glutInitContextVersion(3,3);

	    glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	    //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    /*
	    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE );
	    GLint buf, sample;
	    glGetIntegerv (GL_SAMPLE_BUFFERS, &buf);
	    glGetIntegerv (GL_SAMPLES, &sample);

	    std::cout << "buf: " << buf << " samples: " << sample << " \n";
	    glEnable(GL_MULTISAMPLE);
    */

	    glutInitWindowSize(200,200);
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
		GL_CHECK_ERRORS

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
	
	    renderer->onCreate();

	    glutDisplayFunc(&display);
	    glutReshapeFunc(&reshape);
	    glutKeyboardFunc(&keyboard);
	    glutMouseFunc(&pressed);
	    glutMotionFunc(&dragged);
	    glutPassiveMotionFunc(&moved);
	    glutIdleFunc(&animate);

		lastTime = std::clock();
		//std::time(&lastTime);
	    //gettimeofday(&lastTime, NULL);

	    glutMainLoop();

	    //glutInitDisplayMode(GL_RGBA);
	    //glutInitWindowSize(200,200);
	    //glutCreateWindow("test");

	    return NULL;
	    //return glView;

    }
}

