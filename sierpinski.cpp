#include "pch.h"
#include <windows.h>
#include <time.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

GLint poziom = 7;
GLfloat szerokosc = 100;
GLfloat st_deformacji = 0;
GLint iteracje = 10000000;
// x' = ax + by + c
// y' = dx + ey + f

GLfloat fi[4][6] = { 
	{-0.67, -0.02, 0.00, -0.18,  0.81,  10.00},
	{ 0.40,  0.40, 0.00, -0.10,  0.40,   0.00},
	{-0.40, -0.40, 0.00, -0.10,  0.40,   0.00},
	{-0.10,  0.00, 0.00,  0.44,  0.44,  -2.00}
};

void paprotka(GLfloat x, GLfloat y, GLint iteracje) {

	glPointSize(1.0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
		glVertex2f(x, y);
	glEnd();
	srand(time(NULL));
	for (int i = 0; i < iteracje; i++) {
		GLfloat x1, y1;
		int randIndex = rand() % 4;
			x1 = fi[randIndex][0] * x + fi[randIndex][1] * y + fi[randIndex][2];
			y1 = fi[randIndex][3] * x + fi[randIndex][4] * y + fi[randIndex][5];
			// std::cout << x << " " << y << std::endl;
			glPointSize(1.0);
			glColor3f(0.0f, 1.0f, 0.0f);
			glBegin(GL_POINTS);
				glVertex2f(x1, y1);
			glEnd();
			x = x1; y = y1;
	}
}

void plasma(GLfloat x, GLfloat y, GLfloat szerokosc, GLint poziom) {
	if (poziom > 0) {
		szerokosc = szerokosc / 2;
		plasma(x, y, szerokosc, poziom - 1);
		plasma(x - szerokosc, y, szerokosc, poziom - 1);
		plasma(x, y - szerokosc, szerokosc, poziom - 1);
		plasma(x - szerokosc, y - szerokosc, szerokosc, poziom - 1);

	}
	else {
		glBegin(GL_POLYGON);
		//glColor3f(0.2f, 0.1f, 0.4f);
		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		glVertex2f(x, y);
		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		glVertex2f(x, y - szerokosc);
		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		glVertex2f(x - szerokosc, y - szerokosc);
		glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		glVertex2f(x - szerokosc, y);
		glEnd();
	}
}

void rysuj_dywan(GLfloat x, GLfloat y, GLfloat szerokosc, GLint poziom) {
	if (poziom > 0) {
		szerokosc = szerokosc / 3;
		rysuj_dywan(x, y, szerokosc, poziom - 1);
		rysuj_dywan(x - szerokosc, y, szerokosc, poziom - 1);
		rysuj_dywan(x - 2 * szerokosc, y, szerokosc, poziom - 1);
		rysuj_dywan(x, y - szerokosc, szerokosc, poziom - 1);
		rysuj_dywan(x - 2 * szerokosc, y - szerokosc, szerokosc, poziom - 1);
		rysuj_dywan(x, y - 2 * szerokosc, szerokosc, poziom - 1);
		rysuj_dywan(x - szerokosc, y - 2 * szerokosc, szerokosc, poziom - 1);
		rysuj_dywan(x - 2 * szerokosc, y - 2 * szerokosc, szerokosc, poziom - 1);
	}
	else {
		GLfloat def = (rand() % 20)*st_deformacji / (10 * szerokosc);
		glBegin(GL_POLYGON);
		glColor3f(0.2f, 0.1f, 0.4f);
		//glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		glVertex2f(x + def, y + def);
		//glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		glVertex2f(x + def, y - szerokosc + def);
		//glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		glVertex2f(x - szerokosc + def, y - szerokosc + def);
		//glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		glVertex2f(x - szerokosc + def, y + def);
		glEnd();
	}
}

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0f, 1.0f, 0.0f);

	//plasma(50, 50, szerokosc, poziom);

	paprotka(0, 0, iteracje);

	glFlush();
}

void MyInit(void) {
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {
	GLfloat AspectRatio;

	if (vertical == 0)
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	if (horizontal <= vertical)
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
	else
		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

void main(void) {

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutCreateWindow("Dywan Sierpinskiego");

	glutDisplayFunc(RenderScene);

	glutReshapeFunc(ChangeSize);

	MyInit();

	glutMainLoop();
}
