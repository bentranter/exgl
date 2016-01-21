#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include "erl_nif.h"

char keyboardbuffer[20][40];
int keyboardindex_x[20];
int keyboardindex_y;

/*
 * Sprint prints text to screen. It'll print text at location x, y.
 */
void Sprint(int x, int y, char *s) {
	int l, i;
	l = strlen(s);
	glRasterPos2i(x, y);
	for (i = 0; i < l; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_24, s[i]);
	}
}

/*
 * Init initializes our OpenGL world. It makes the screen black, sets the
 * object to be rendered smotth, and checks the depth when rendering.
 */
static void init() {
	int i;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	for (i = 0; i < 20; i++) {
		keyboardindex_x[i] = 0;
	}
	keyboardindex_y = 8;
}

/*
 * Update updates the screen with new data, then resets the timer.
 */
static void update() {
	glutPostRedisplay();
	glutTimerFunc(100, update, 1);
}

/*
 * Display displays our scene. It sets up the camera and tells OpenGL we're
 * we're drawing.
 */
static void display() {
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8.0, 8.0, -8.0, 8.0, 0.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0, 1.0, 1.0);
	for ( i = 0; i < 20; i++) {
		Sprint(-8, 8 - i, keyboardbuffer[i]);
	}

	glutSwapBuffers();
}

/*
 * Resize_window handles the window resizing.
 */
void resize_window(int w, int h) {
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

/*
 * Handled some keyboard stuff, obv useless now.
 */
void arrows(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		keyboardindex_y--;
		if (keyboardindex_y < 0) keyboardindex_y = 0;
	}

	if (key == GLUT_KEY_DOWN) {
		keyboardindex_y++;
		if (keyboardindex_y > 19) keyboardindex_y = 19;
	}
}

// Read the keyboard
void keyboard (unsigned char key, int x, int y) {
	// Check for a letter of the alphabet
	if ( isalnum(key) || key == ' ') {
    	keyboardbuffer[keyboardindex_y][keyboardindex_x[keyboardindex_y]] = key;
    	keyboardindex_x[keyboardindex_y]++;
    	// Prevent buffer overflow
    	if (keyboardindex_x[keyboardindex_y] > 39 ) keyboardindex_x[keyboardindex_y] = 39;
    }

	if (key == 8) {
		keyboardindex_x[keyboardindex_y]--;
		if ( keyboardindex_x[keyboardindex_y] < 0) keyboardindex_x[keyboardindex_y] = 0;
		keyboardbuffer[keyboardindex_y][keyboardindex_x[keyboardindex_y]] = 0;
	}

	// This is useless because we can't read form the keyboard since we start
	// the code from within the BEAM...
	switch (key) {
		case 27:
			exit(0);
			break;
		default:
			break;
	}
}

/*
 * Start is called from Elixir, and starts and runs our OpenGL code.
 */
static ERL_NIF_TERM start(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (500, 500);
	glutInitWindowPosition (10, 10);
	glutCreateWindow (argv[0]);
	glutSetWindowTitle("OpenGL in Elixir");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize_window);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrows);
	glutTimerFunc(10, update, 1);

	// LMAO just block the BEAM forever.
	glutMainLoop();
	return enif_make_int(env, 0);
}

static ERL_NIF_TERM printscr(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
	char buf[256];
	if (!enif_get_string(env, argv[0], buf, 256, ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
	}

	Sprint(0, 0, buf);
	printf("Hello, %s\n", buf);
	return enif_make_int(env, 0);
}

/*
 * Register our NIF functions.
 */
static ErlNifFunc nif_funcs[] = {
	{"start", 0, start},
	{"printscr", 1, printscr}
};

/*
 * Register our library.
 */
ERL_NIF_INIT(Elixir.GL, nif_funcs, NULL, NULL, NULL, NULL)
