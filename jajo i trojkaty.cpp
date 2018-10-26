#include "pch.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>

#define M_PI 3.14159265358979323846

typedef float point3[3];
static GLfloat theta[] = { 0.0, 0.0, 0.0 };
static GLint status = 0;
using namespace std;

void Axes(void) {
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}

float getX(float u, float v) {
	float PIV = M_PI * v;
	return ((-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(PIV));
}
float getY(float u, float v) {
	return (160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2));
}
float getZ(float u, float v) {
	float PIV = M_PI * v;
	return ((-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(PIV));
}

struct Punkt {
	float x;
	float y;
	float z;
	float xRGB;
	float yRGB;
	float zRGB;
};

// 1- punkty, 2- siatka, 3 - wypełnione trójkąty
int model = 3;
int movement = 1;
float KAT = 90.0;

// Ilosc podzialow boku kwadratu jednostkowego
const int N = 50;

struct Punkt eggPoints[N][N];

float krok = 1.0 / N;

void EggPtCount() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			eggPoints[i][j].x = getX(i * krok, j * krok);
			eggPoints[i][j].y = getY(i * krok, j * krok);
			eggPoints[i][j].z = getZ(i * krok, j * krok);

			eggPoints[i][j].xRGB = ((double)rand() / (RAND_MAX));
			eggPoints[i][j].yRGB = ((double)rand() / (RAND_MAX));
			eggPoints[i][j].zRGB = ((double)rand() / (RAND_MAX));
		}
	}
}

void Egg() {
	switch (model) {
		// Punkty
	case 1:
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				glBegin(GL_POINTS);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
				glEnd();
			}
		}
		break;
		// Linie
	case 2:
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		// Poziome
		for (int i = 0; i < N; i++) {
			int pom = N - i;
			for (int j = 0; j < N - 1; j++) {
				glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
				glVertex3f(eggPoints[i][j + 1].x, eggPoints[i][j + 1].y - 5, eggPoints[i][j + 1].z);
			}
			if (pom != N) {
				glVertex3f(eggPoints[i][0].x, eggPoints[i][0].y - 5, eggPoints[i][0].z);
				glVertex3f(eggPoints[pom][N - 1].x, eggPoints[pom][N - 1].y - 5, eggPoints[pom][N - 1].z);
			}
		}

		// Poionowe
		glColor3f(1.0f, 1.0f, 0.0f);
		for (int i = 0; i < N; i++) {
			int pom = N - i;
			for (int j = 0; j < N; j++) {
				if (i == N - 1) {
					glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
					glVertex3f(eggPoints[0][0].x, eggPoints[0][0].y - 5, eggPoints[0][0].z);
				}
				else {
					glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
					glVertex3f(eggPoints[i + 1][j].x, eggPoints[i + 1][j].y - 5, eggPoints[i + 1][j].z);
				}
			}
		}

		// Skośne
		glColor3f(0.294f, 0.0f, 0.510f);
		for (int i = N / 2; i < N; i++) {
			int pom = N - i;
			for (int j = 0; j < N - 1; j++) {
				if (i == N - 1) {
					glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
					glVertex3f(eggPoints[0][0].x, eggPoints[0][0].y - 5, eggPoints[0][0].z);
				}
				else {
					glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
					glVertex3f(eggPoints[i + 1][j + 1].x, eggPoints[i + 1][j + 1].y - 5, eggPoints[i + 1][j + 1].z);
				}
				if (pom != N) {
					glVertex3f(eggPoints[i][0].x, eggPoints[i][0].y - 5, eggPoints[i][0].z);
					glVertex3f(eggPoints[pom + 1][N - 1].x, eggPoints[pom + 1][N - 1].y - 5, eggPoints[pom + 1][N - 1].z);
				}
			}
		}
		glVertex3f(eggPoints[0][0].x, eggPoints[0][0].y - 5, eggPoints[0][0].z);
		glVertex3f(eggPoints[1][N - 1].x, eggPoints[1][N - 1].y - 5, eggPoints[1][N - 1].z);

		glVertex3f(eggPoints[0][0].x, eggPoints[0][0].y - 5, eggPoints[0][0].z);
		glVertex3f(eggPoints[N - 1][N - 1].x, eggPoints[N - 1][N - 1].y - 5, eggPoints[N - 1][N - 1].z);
		for (int i = 1; i < N / 2 + 1; i++) {
			int pom = N - i;
			for (int j = 0; j < N - 1; j++) {
				glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
				glVertex3f(eggPoints[i - 1][j + 1].x, eggPoints[i - 1][j + 1].y - 5, eggPoints[i - 1][j + 1].z);
				if (pom != N) {
					glVertex3f(eggPoints[i][0].x, eggPoints[i][0].y - 5, eggPoints[i][0].z);
					glVertex3f(eggPoints[pom - 1][N - 1].x, eggPoints[pom - 1][N - 1].y - 5, eggPoints[pom - 1][N - 1].z);
				}
			}
		}

		glEnd();
		break;
		// Kolor
	case 3:
	glBegin(GL_TRIANGLES);
	for (int i = N / 2; i < N; i++) {
		int pom = N - i;
		for (int j = 0; j < N - 1; j++) {
			if (i == N - 1) {
				glColor3f(eggPoints[i][j].xRGB, eggPoints[i][j].yRGB, eggPoints[i][j].zRGB);
				glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
				glColor3f(eggPoints[i][j + 1].xRGB, eggPoints[i][j + 1].yRGB, eggPoints[i][j + 1].zRGB);
				glVertex3f(eggPoints[i][j + 1].x, eggPoints[i][j + 1].y - 5, eggPoints[i][j + 1].z);
				glColor3f(eggPoints[0][0].xRGB, eggPoints[0][0].yRGB, eggPoints[0][0].zRGB);
				glVertex3f(eggPoints[0][0].x, eggPoints[0][0].y - 5, eggPoints[0][0].z);
			}
			else {
				glColor3f(eggPoints[i][j].xRGB, eggPoints[i][j].yRGB, eggPoints[i][j].zRGB);
				glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
				glColor3f(eggPoints[i + 1][j].xRGB, eggPoints[i + 1][j].yRGB, eggPoints[i + 1][j].zRGB);
				glVertex3f(eggPoints[i + 1][j].x, eggPoints[i + 1][j].y - 5, eggPoints[i + 1][j].z);
				glColor3f(eggPoints[i + 1][j + 1].xRGB, eggPoints[i + 1][j + 1].yRGB, eggPoints[i + 1][j + 1].zRGB);
				glVertex3f(eggPoints[i + 1][j + 1].x, eggPoints[i + 1][j + 1].y - 5, eggPoints[i + 1][j + 1].z);

				glColor3f(eggPoints[i][j].xRGB, eggPoints[i][j].yRGB, eggPoints[i][j].zRGB);
				glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
				glColor3f(eggPoints[i][j + 1].xRGB, eggPoints[i][j + 1].yRGB, eggPoints[i][j + 1].zRGB);
				glVertex3f(eggPoints[i][j + 1].x, eggPoints[i][j + 1].y - 5, eggPoints[i][j + 1].z);
				glColor3f(eggPoints[i + 1][j + 1].xRGB, eggPoints[i + 1][j + 1].yRGB, eggPoints[i + 1][j + 1].zRGB);
				glVertex3f(eggPoints[i + 1][j + 1].x, eggPoints[i + 1][j + 1].y - 5, eggPoints[i + 1][j + 1].z);
			}
			if (pom != N) {
				glColor3f(eggPoints[i][0].xRGB, eggPoints[i][0].yRGB, eggPoints[i][0].zRGB);
				glVertex3f(eggPoints[i][0].x, eggPoints[i][0].y - 5, eggPoints[i][0].z);
				glColor3f(eggPoints[pom][N - 1].xRGB, eggPoints[pom][N - 1].yRGB, eggPoints[pom][N - 1].zRGB);
				glVertex3f(eggPoints[pom][N - 1].x, eggPoints[pom][N - 1].y - 5, eggPoints[pom][N - 1].z);
				glColor3f(eggPoints[pom + 1][N - 1].xRGB, eggPoints[pom + 1][N - 1].yRGB, eggPoints[pom + 1][N - 1].zRGB);
				glVertex3f(eggPoints[pom + 1][N - 1].x, eggPoints[pom + 1][N - 1].y - 5, eggPoints[pom + 1][N - 1].z);

				if (i != N / 2) {
					glColor3f(eggPoints[i][0].xRGB, eggPoints[i][0].yRGB, eggPoints[i][0].zRGB);
					glVertex3f(eggPoints[i][0].x, eggPoints[i][0].y - 5, eggPoints[i][0].z);
					glColor3f(eggPoints[i - 1][0].xRGB, eggPoints[i - 1][0].yRGB, eggPoints[i - 1][0].zRGB);
					glVertex3f(eggPoints[i - 1][0].x, eggPoints[i - 1][0].y - 5, eggPoints[i - 1][0].z);
					glColor3f(eggPoints[pom + 1][N - 1].xRGB, eggPoints[pom + 1][N - 1].yRGB, eggPoints[pom + 1][N - 1].zRGB);
					glVertex3f(eggPoints[pom + 1][N - 1].x, eggPoints[pom + 1][N - 1].y - 5, eggPoints[pom + 1][N - 1].z);
				}
			}
		}
	}
	glColor3f(eggPoints[0][0].xRGB, eggPoints[0][0].yRGB, eggPoints[0][0].zRGB);
	glVertex3f(eggPoints[0][0].x, eggPoints[0][0].y - 5, eggPoints[0][0].z);
	glColor3f(eggPoints[N - 1][0].xRGB, eggPoints[N - 1][0].yRGB, eggPoints[N - 1][0].zRGB);
	glVertex3f(eggPoints[N - 1][0].x, eggPoints[N - 1][0].y - 5, eggPoints[N - 1][0].z);
	glColor3f(eggPoints[1][N - 1].xRGB, eggPoints[1][N - 1].yRGB, eggPoints[1][N - 1].zRGB);
	glVertex3f(eggPoints[1][N - 1].x, eggPoints[1][N - 1].y - 5, eggPoints[1][N - 1].z);


	glColor3f(eggPoints[0][0].xRGB, eggPoints[0][0].yRGB, eggPoints[0][0].zRGB);
	glVertex3f(eggPoints[0][0].x, eggPoints[0][0].y - 5, eggPoints[0][0].z);
	glColor3f(eggPoints[1][0].xRGB, eggPoints[1][0].yRGB, eggPoints[1][0].zRGB);
	glVertex3f(eggPoints[1][0].x, eggPoints[1][0].y - 5, eggPoints[1][0].z);
	glColor3f(eggPoints[N - 1][N - 1].xRGB, eggPoints[N - 1][N - 1].yRGB, eggPoints[N - 1][N - 1].zRGB);
	glVertex3f(eggPoints[N - 1][N - 1].x, eggPoints[N - 1][N - 1].y - 5, eggPoints[N - 1][N - 1].z);


	for (int i = 0; i < N / 2 + 1; i++) {
		int pom = N - i;
		for (int j = 0; j < N - 1; j++) {
			if (i == 1) {
				glColor3f(eggPoints[i][j].xRGB, eggPoints[i][j].yRGB, eggPoints[i][j].zRGB);
				glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
				glColor3f(eggPoints[i][j + 1].xRGB, eggPoints[i][j + 1].yRGB, eggPoints[i][j + 1].zRGB);
				glVertex3f(eggPoints[i][j + 1].x, eggPoints[i][j + 1].y - 5, eggPoints[i][j + 1].z);
				glColor3f(eggPoints[0][0].xRGB, eggPoints[0][0].yRGB, eggPoints[0][0].zRGB);
				glVertex3f(eggPoints[0][0].x, eggPoints[0][0].y - 5, eggPoints[0][0].z);
			}
			else {
				if (i != 0) {
					glColor3f(eggPoints[i][j].xRGB, eggPoints[i][j].yRGB, eggPoints[i][j].zRGB);
					glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
					glColor3f(eggPoints[i - 1][j].xRGB, eggPoints[i - 1][j].yRGB, eggPoints[i - 1][j].zRGB);
					glVertex3f(eggPoints[i - 1][j].x, eggPoints[i - 1][j].y - 5, eggPoints[i - 1][j].z);
					glColor3f(eggPoints[i - 1][j + 1].xRGB, eggPoints[i - 1][j + 1].yRGB, eggPoints[i - 1][j + 1].zRGB);
					glVertex3f(eggPoints[i - 1][j + 1].x, eggPoints[i - 1][j + 1].y - 5, eggPoints[i - 1][j + 1].z);
				}


				glColor3f(eggPoints[i][j].xRGB, eggPoints[i][j].yRGB, eggPoints[i][j].zRGB);
				glVertex3f(eggPoints[i][j].x, eggPoints[i][j].y - 5, eggPoints[i][j].z);
				glColor3f(eggPoints[i][j + 1].xRGB, eggPoints[i][j + 1].yRGB, eggPoints[i][j + 1].zRGB);
				glVertex3f(eggPoints[i][j + 1].x, eggPoints[i][j + 1].y - 5, eggPoints[i][j + 1].z);
				glColor3f(eggPoints[i - 1][j + 1].xRGB, eggPoints[i - 1][j + 1].yRGB, eggPoints[i - 1][j + 1].zRGB);
				glVertex3f(eggPoints[i - 1][j + 1].x, eggPoints[i - 1][j + 1].y - 5, eggPoints[i - 1][j + 1].z);
			}
			if (pom != N) {
				glColor3f(eggPoints[i][0].xRGB, eggPoints[i][0].yRGB, eggPoints[i][0].zRGB);
				glVertex3f(eggPoints[i][0].x, eggPoints[i][0].y - 5, eggPoints[i][0].z);
				glColor3f(eggPoints[pom][N - 1].xRGB, eggPoints[pom][N - 1].yRGB, eggPoints[pom][N - 1].zRGB);
				glVertex3f(eggPoints[pom][N - 1].x, eggPoints[pom][N - 1].y - 5, eggPoints[pom][N - 1].z);
				glColor3f(eggPoints[pom - 1][N - 1].xRGB, eggPoints[pom - 1][N - 1].yRGB, eggPoints[pom - 1][N - 1].zRGB);
				glVertex3f(eggPoints[pom - 1][N - 1].x, eggPoints[pom - 1][N - 1].y - 5, eggPoints[pom - 1][N - 1].z);


				glColor3f(eggPoints[i][0].xRGB, eggPoints[i][0].yRGB, eggPoints[i][0].zRGB);
				glVertex3f(eggPoints[i][0].x, eggPoints[i][0].y - 5, eggPoints[i][0].z);
				glColor3f(eggPoints[i + 1][0].xRGB, eggPoints[i + 1][0].yRGB, eggPoints[i + 1][0].zRGB);
				glVertex3f(eggPoints[i + 1][0].x, eggPoints[i + 1][0].y - 5, eggPoints[i + 1][0].z);
				glColor3f(eggPoints[pom - 1][N - 1].xRGB, eggPoints[pom - 1][N - 1].yRGB, eggPoints[pom - 1][N - 1].zRGB);
				glVertex3f(eggPoints[pom - 1][N - 1].x, eggPoints[pom - 1][N - 1].y - 5, eggPoints[pom - 1][N - 1].z);
			}
		}
	}
	glEnd();
	break;
	}

}

bool spinning = true;

void Sierpinski(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLint level) {
	if (level > 0) {
		Sierpinski(x, y + width / 2, z, width / 2, level-1);
		Sierpinski(x - width / 4, y, z + width / 4, width / 2, level-1);
		Sierpinski(x + width / 4, y, z + width / 4, width / 2, level-1);
		Sierpinski(x - width / 4, y, z - width / 4, width / 2, level-1);
		Sierpinski(x + width / 4, y, z - width / 4, width / 2, level-1);
	}
	else {
		glBegin(GL_TRIANGLES);
		glColor3f(0.72f, 0.53f, 0.04f);

		//glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		//face 1
		glVertex3f(x - width / 2, y, z + width / 2);
		glVertex3f(x + width / 2, y, z + width / 2);
		glVertex3f(x, y + width, z);

		glColor3f(0.8f, 0.68f, 0.0f);
		//glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		//face 2
		glVertex3f(x - width / 2, y, z - width / 2);
		glVertex3f(x + width / 2, y, z - width / 2);
		glVertex3f(x, y + width, z);

		glColor3f(0.85f, 0.85f, 0.1f);
		//glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		//face 3
		glVertex3f(x + width / 2, y, z + width / 2);
		glVertex3f(x + width / 2, y, z - width / 2);
		glVertex3f(x, y + width, z);

		glColor3f(0.88f, 0.85f, 0.45f);
		//glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		//face 4
		glVertex3f(x - width / 2, y, z - width / 2);
		glVertex3f(x - width / 2, y, z + width / 2);
		glVertex3f(x, y + width, z);

		glColor3f(0.93f, 0.60f, 0.0f);
		//glColor3f(((rand() % 100)*0.01), ((rand() % 100)*0.01), ((rand() % 100)*0.01));
		glVertex3f(x - width / 2, y, z - width / 2);
		glVertex3f(x + width / 2, y, z - width / 2);
		glVertex3f(x - width / 2, y, z + width / 2);
		

		glVertex3f(x + width / 2, y, z - width / 2);
		glVertex3f(x - width / 2, y, z + width / 2);
		glVertex3f(x + width / 2, y, z + width / 2);
		glEnd();
	}
}

void spinEgg() {
	if (spinning) {
		theta[0]--;
		if (theta[0] > 360.0) theta[0] -= 360.0;
		theta[1]--;
		if (theta[1] > 360.0) theta[1] -= 360.0;
		theta[2]--;
		if (theta[2] > 360.0) theta[2] -= 360.0;
		glutPostRedisplay();
	}
}

void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);
	Axes();
	//Egg();
	Sierpinski(0.0, -3.0, 0.0, 8, 3);
	glFlush();
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y) {
	if (key == '1') model = 1;
	if (key == '2') model = 2;
	if (key == '3') model = 3;
	if (key == '`') spinning = !spinning;
	if (key == 'r') {
		theta[0]+=3;
		theta[1]+=3;
		theta[2]+=3;
	}

	RenderScene();
}

void MyInit(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
		glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);
	else
		glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	EggPtCount();
	glutCreateWindow("OprnGL - modelowanie obiektów 3D");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutIdleFunc(spinEgg);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutKeyboardFunc(keys);
	glutMainLoop();
}
