/*
 *  Se folosesc evaluatorii pentru reprezentarea unei suprafete curbe cu generarea automata a coordonatelor de texturare
 */
#include "glos.h"

#include "gl.h"
#include "glu.h"
#include "glaux.h"
#include <math.h>

GLfloat ctrlpoints[4][4][3] = {
	{{ -1.5, -1.5, 4.0}, { -0.5, -1.5, 2.0},
	{0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}},
	{{ -1.5, -0.5, 1.0}, { -0.5, -0.5, 3.0},
	{0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}},
	{{ -1.5, 0.5, 4.0}, { -0.5, 0.5, 0.0},
	{0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}},
	{{ -1.5, 1.5, -2.0}, { -0.5, 1.5, -2.0},
	{0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};

GLfloat texturePoints[2][2][2] = { {{0.0, 0.0}, {0.0, 1.0}},  {{1.0, 0.0}, {1.0, 1.0}} }; // punctele de control pentru textura

// se construieste textura
#define imageWidth 64
#define imageHeight 64
GLubyte image[3 * imageWidth * imageHeight];

void makeImage()
{
	for (int i = 0; i < imageWidth; i++) {
		float ti = 2.0 * 3.14159265 * i / imageWidth;
		for (int j = 0; j < imageHeight; j++) {
			float tj = 2.0 * 3.14159265 * j / imageHeight;

			image[3 * (imageHeight * i + j)] = (GLubyte)127 * (1.0 + sin(ti));
			image[3 * (imageHeight * i + j) + 1] = (GLubyte)127 * (1.0 + cos(2 * tj));
			image[3 * (imageHeight * i + j) + 2] = (GLubyte)127 * (1.0 + cos(ti + tj));
		}
	}
}

void myInit()
{
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);

	makeImage();

	// culoarea texturii este plasata pe obiect fara a se tine sema de iluminare
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// se repeta texturarea pe ambele directii
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// se specifica textura
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth, imageHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glEnable(GL_TEXTURE_2D); // activeaza texturarea 2D
	glEnable(GL_DEPTH_TEST); // activeaza ascunderea suprafetelor
	glEnable(GL_NORMALIZE); // se normalizeaza vectorii normalelor
}

void CALLBACK display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(85.0, 1.0, 1.0, 1.0);

	glColor3f(1.0, 1.0, 1.0);

	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texturePoints[0][0][0]);

	// aici are loc desenarea suprafetei texturarea fiind activata suprafata este desenata texturata
	glEvalMesh2(GL_FILL, 0, 20, 0, 20);

	glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h) {
		glOrtho(-4.0, 4.0, -4.0 * (GLfloat)h / (GLfloat)w, 4.0 * (GLfloat)h / (GLfloat)w, -4.0, 4.0);
	}
	else {
		glOrtho(-4.0 * (GLfloat)w / (GLfloat)h, 4.0 * (GLfloat)w / (GLfloat)h, -4.0, 4.0, -4.0, 4.0);
	}
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	auxInitDisplayMode(AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
	auxInitPosition(0, 0, 800, 600);
	auxInitWindow("Texturarea suprafetelor generate cu evaluatori");
	myInit();
	auxReshapeFunc(myReshape);
	auxMainLoop(display);
	return 0;
}
