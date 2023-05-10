/*
 * Acest program mapeaza o textura pe un ceainic cu generarea automata a coordonatelor de texturare.
 * Textura este redata ca hasuri pe ceainic.
 */
#include "glos.h"

#include "gl.h"
#include "glu.h"
#include "glaux.h"

static int angle = 0;

#define stripeImageWidth 32
GLubyte stripeImage[3 * stripeImageWidth];

// construieste o textura 1D cu 3 octeti pentru fiecare texel din 32 texeli 5 sunt rosii si 27 sunt verzi
void makeStripeImage(void)
{
	for (int j = 0; j < stripeImageWidth; j++) {
		stripeImage[3 * j] = (j <= 4) ? 255 : 0; // rosu
		stripeImage[3 * j] = (j > 4) ? 255 : 0; // verde
		stripeImage[3 * j + 2] = 0; // galben
	}
}

void myInit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	makeStripeImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// incercati si parametrul GL_DECAL. Veti constata ca nu se tine seama de iluminare
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// incercati si cu GL_CLAMP. Nu se repeta texturarea. Se prelungec aceiasi texeli
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// incercati si GL_OBJECT_PLANE
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_PLANE);

	// coeficientii planului fata de care se calculeaza directia texturii
	GLfloat sGenParams[] = { 0.0, 0.0, 1.0, 0.0 };
	// incercati si parametrul GL_EYE_PLANE
	glTexGenfv(GL_S, GL_EYE_PLANE, sGenParams);

	glTexImage1D(GL_TEXTURE_1D, 0, 3, stripeImageWidth,
		0, GL_RGB, GL_UNSIGNED_BYTE, stripeImage);


	// ascunderea suprafetelor
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_GEN_S); // se genereaza automat coordonatele texturii pe s
	glEnable(GL_TEXTURE_1D); // texturare 1D

	// iluminarea
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// calcularea automata a normalelor pentru coordonate generate cu GL_MAP1_VERTEX
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0); // exponentul pentru stralucire


}

void CALLBACK display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(angle, 0.0, 1.0, 1.0);
	
	auxSolidCube(2.0);

	glFlush();
}

void CALLBACK IdleFunction()
{
	angle += 1;
	display();
	Sleep(1000 / 60);
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h) {
		glOrtho(-3.5, 3.5, -3.5 * (GLfloat)h / (GLfloat)w, 3.5 * (GLfloat)h / (GLfloat)w, -3.5, 3.5);
	}
	else {
		glOrtho(-3.5 * (GLfloat)w / (GLfloat)h, 3.5 * (GLfloat)w / (GLfloat)h, -3.5, 3.5, -3.5, 3.5);
	}
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	auxInitDisplayMode(AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
	auxInitPosition(0, 0, 800, 600);
	auxInitWindow("Texture Map");
	myInit();
	auxReshapeFunc(myReshape);
	auxIdleFunc(IdleFunction);
	auxMainLoop(display);
	return 0;
}
