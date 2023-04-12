/*
 * Programul mapeaza o imagine-tabla de sah-pe doua dreptunghiuri.
 * Acest program blocheaza  (GL_CLAMP) texturarea daca coordonatele texturii (s, t) depasesc valorile 0 respectiv 1.
 */
#include "glos.h"

#include "gl.h"
#include "glu.h"
#include "glaux.h"

 // se creaza imaginea texturii, in interiorul programului, ca tabla de sah
#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageHeight][checkImageWidth][3];

void makeCheckImage()
{
	for (int i = 0; i < checkImageHeight; i++) {
		for (int j = 0; j < checkImageWidth; j++) {
			// culoare alb sau negru deoarece se alege aceiasi valoare pentru R, G, B - 0 sau 255.
			GLubyte c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = c;
			checkImage[i][j][1] = c;
			checkImage[i][j][2] = c;
		}
	}
}

void myInit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	makeCheckImage(); // se initializeaza tabloul care contine imaginea

	// seteaza modul de memorare al pixelilor
	// este necesara alinierea la inceputul fiecarui rand, valoare: 1 octet
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// specificarea modului de aplicare al texturii
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// desenarea obiectelor cu specificarea coordonatelor de texturare
	glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth, checkImageHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, &checkImage[0][0][0]);

	glEnable(GL_TEXTURE_2D);
}

void CALLBACK display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.6);

	glBegin(GL_QUADS);
	{
		// primul patrulater (patrat cu latura de 2)
		glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

		// al doilea patrulater (patrat cu latura de 2)
		glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
		glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
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
	auxInitWindow("Maparea texturilor 3D");
	myInit();
	auxReshapeFunc(myReshape);
	auxMainLoop(display);
	return 0;
}
