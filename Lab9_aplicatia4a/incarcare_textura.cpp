/*
 *  Programul mapeaza texturi diferite, incarcate din fisiere *.BMP,
 *  pe doua patrate.
 */
#include "glos.h"

#include "gl.h"
#include "glu.h"
#include "glaux.h"

GLuint textureId1;
GLuint textureId2;

GLuint incarcaTextura(const char* s)
{
	GLuint textureId = 0;
	AUX_RGBImageRec* pImagineTextura = auxDIBImageLoad(s);

	if (pImagineTextura != NULL)
	{
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
	}
	if (pImagineTextura)
	{
		if (pImagineTextura->data) {
			free(pImagineTextura->data);
		}
		free(pImagineTextura);
	}
	return textureId;
}

void myInit()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	textureId1 = incarcaTextura("covor.bmp");
	textureId2 = incarcaTextura("parchet.bmp");

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
}

void CALLBACK display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);

	// primul patrat
	glBindTexture(GL_TEXTURE_2D, textureId1);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0, 0.0); glVertex3f(-2.5, -1.0, 0.0);
		glTexCoord2f(0.0, 2.0); glVertex3f(-2.5, 1.0, 0.0);
		glTexCoord2f(2.0, 2.0); glVertex3f(-0.5, 1.0, 0.0);
		glTexCoord2f(2.0, 0.0); glVertex3f(-0.5, -1.0, 0.0);
	}
	glEnd();

	// al doilea patrat
	glBindTexture(GL_TEXTURE_2D, textureId2);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0.0, 0.0); glVertex3f(0.5, -1.0, 0.0);
		glTexCoord2f(0.0, 2.0); glVertex3f(0.5, 1.0, 0.0);
		glTexCoord2f(2.0, 2.0); glVertex3f(2.5, 1.0, 0);
		glTexCoord2f(2.0, 0.0); glVertex3f(2.5, -1.0, 0);
	}
	glEnd();

	glFlush();
}

void CALLBACK myReshape(GLsizei w, GLsizei h)
{
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0 * (GLfloat)w / (GLfloat)h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	auxInitDisplayMode(AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
	auxInitPosition(0, 0, 800, 600);
	auxInitWindow("Incarcarea texturilor din fisiere BMP");
	myInit();
	auxReshapeFunc(myReshape);
	auxMainLoop(display);
	return 0;
}





