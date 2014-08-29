//#include "FreeGlutGLView.hpp"
#include "Aluminum/Includes.hpp"
#include <iostream>
#include <sys/time.h>

//namespace Aluminum {

int width;
int height;
RendererLinux* renderer;
static struct timeval lastTime;

FreeGlutGLView::FreeGlutGLView() {}

void reshape(GLint _w, GLint _h) {
	width = _w;
	height = _h;

	renderer->width = width;
	renderer->height = height;

	glViewport(0,0,width,height);

	printf("in reshape: %d %d\n", width, height);
	GLuint bbb;
	glGenVertexArrays(1, &bbb);
}

void display() {

	//printf("in FreeGlutGLView : display()\n");
	renderer->onFrame();
	glutSwapBuffers();
}

void animate() {
	float dt;

	struct timeval now;
	gettimeofday(&now, NULL);
	dt = (float)(now.tv_usec - lastTime.tv_usec);
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

	renderer = (RendererLinux*) _renderer;

	/* annoying useless setup for glutInit */
	char* argv[] = {strdup(name.c_str())};
	int argc = 1;

	glutInit(&argc, argv);
	glutInitContextVersion(3,2);
	//glutInitContextVersion(4,4);

	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
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

	gettimeofday(&lastTime, NULL);

	glutMainLoop();

	//glutInitDisplayMode(GL_RGBA);
	//glutInitWindowSize(200,200);
	//glutCreateWindow("test");

	return NULL;
	//return glView;

}

//}
