#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>

//chyba można po prostu użyć math.h tutaj
#define M_PI 3.14159265358979323846

static GLfloat thetaY = 0.0;   // kąt obrotu obiektu
static GLfloat thetaX = 0.0;

static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy 
							   // 0 - nie naciśnięto żadnego klawisza
							   // 1 - naciśnięty zostać lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int z_pos_old = 1;
static int righty = 0;

static int delta_x = 0;        // różnica pomiędzy pozycją bieżącą
									  // i poprzednią kursora myszy 
static int delta_y = 0;
static float delta_z = 1.0;

float r = 10.0;

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
using namespace std;

void Mouse(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
		y_pos_old = y;			// jako pozycji poprzedniej

		status = 1;          // wcięnięty został lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		y_pos_old = y;
		status = 2;
	}
	else {
		status = 0;
	}
}


void Motion(GLsizei x, GLsizei y) {

	delta_x = x - x_pos_old;     // obliczenie różnicy położenia kursora myszy
	delta_y = y - y_pos_old;

	x_pos_old = x;            // podstawienie bieżącego położenia jako poprzednie
	y_pos_old = y;

	glutPostRedisplay();     // przerysowanie obrazu sceny
}

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

//1 - punkty
//2 - siatka
//3 - wypełnione
int model = 3;
int movement = 1;
float KAT = 90.0;

//ilość podzialow boku kwadratu jednostkowego
const int N = 50;

struct Punkt eggPoints[N][N];

float krok = 1.0 / N;
float theta = 0.0;
float fi = 0.0;




void EggPtCount() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {

			//wyliczanie odpowienich koordynat punktów jajka
			eggPoints[i][j].x = getX(i * krok, j * krok);
			eggPoints[i][j].y = getY(i * krok, j * krok);
			eggPoints[i][j].z = getZ(i * krok, j * krok);

			//losowanie kolorów
			eggPoints[i][j].xRGB = ((double)rand() / (RAND_MAX));
			eggPoints[i][j].yRGB = ((double)rand() / (RAND_MAX));
			eggPoints[i][j].zRGB = ((double)rand() / (RAND_MAX));
		}
	}
}

void Egg() {
	switch (model) {

		//punkty
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

		//linie
	case 2:
		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);

		//poziome
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

		//pionowe
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

		//na skos
		glColor3f(0.3f, 0.0f, 0.40f);
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

		//kolor
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

void Sierpinski(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLint level) {
	if (level > 0) {
		Sierpinski(x, y + width / 2, z, width / 2, level - 1);
		Sierpinski(x - width / 4, y, z + width / 4, width / 2, level - 1);
		Sierpinski(x + width / 4, y, z + width / 4, width / 2, level - 1);
		Sierpinski(x - width / 4, y, z - width / 4, width / 2, level - 1);
		Sierpinski(x + width / 4, y, z - width / 4, width / 2, level - 1);
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

// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x2 = 0.0f, z2 = 5.0f;

static float centerX = 0.0, centerY = 0.0, centerZ = 0.0;

static bool zoombarrier = true;
static int modelek = 1;

void RenderScene(void) {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], centerX, centerY, centerZ, 0.0, 1.0, 0.0);
	//gluLookAt(x2, 1.0f, z2, x2 + lx, 1.0f, z2 + lz, 0.0f, 1.0f, 0.0f);
	Axes();
	viewer[0] = r * cos(theta)*cos(fi);
	viewer[1] = r * sin(fi);
	viewer[2] = r * sin(theta)*cos(fi);

	if (status == 1) {
		//thetaY += delta_x * pix2angle;    // modyfikacja kąta obrotu o kat proporcjonalny
		//thetaX += delta_y * pix2angle;


		theta += delta_x * 0.05 * pix2angle;
		fi += delta_y * 0.05 * pix2angle;
	}
	else if (status == 2) {
		//delta_z += delta_y * 0.01;
		if (zoombarrier == true) {
			if (r < 7) {
				r = 7;
			}
			else {
				r += delta_y * 0.01;
			}
		}
		else {
			r += delta_y * 0.01;
		}

	}
	// do różnicy położeń kursora myszy

//glRotatef(thetaY, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt
//glRotatef(thetaX, 1.0, 0.0, 0.0);  // do różnicy położeń kursora myszy
//glScalef(delta_z, delta_z, delta_z);

//zakomentować niepotrzebne:
	if (modelek == 1) {
		Egg();
		zoombarrier = true;
	}
	else {
		Sierpinski(0.0, -3.0, 0.0, 8, 4);
		zoombarrier = false;
	}

	//koordynaty początkowe x y z, szerokość, poziom
	
	//glColor3f(1.0f, 1.0f, 1.0f);
	// Ustawienie koloru rysowania na biały

	//glutWireTeapot(3.0);
	// Narysowanie czajnika

	glFlush();
	glutSwapBuffers();
}


//sterowanie wyswietlanym modelem
void keys(unsigned char key, int x, int y) {
	if (key == '1') model = 1;
	if (key == '2') model = 2;
	if (key == '3') model = 3;
	if (key == '4') {
		if (modelek == 1) modelek = 2;
		else modelek = 1;
	}
	if (key == 'w') {
		centerY += 0.1;
	}
	if (key == 's') {
		centerY -= 0.1;
	}
	if (key == 'a') {
		centerZ += 0.1;
	}
	if (key == 'd') {
		centerZ -= 0.1;
	}
	RenderScene();
}

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.1f;

	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x2 += lx * fraction;
		z2 += lz * fraction;
		break;
	case GLUT_KEY_DOWN:
		x2 -= lx * fraction;
		z2 -= lz * fraction;
		break;
	}
}

void MyInit(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {

	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie

	GLfloat AspectRatio;
	if (vertical == 0)
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(70, 1.0, 1.0, 30.0);

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	EggPtCount();
	glutCreateWindow("Ćwiczenie 4");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutKeyboardFunc(keys);
	glutMouseFunc(Mouse);
	// Ustala funkcję zwrotną odpowiedzialną za badanie stanu myszy

	glutMotionFunc(Motion);
	//glutKeyboardFunc(processNormalKeys);
	//glutSpecialFunc(processSpecialKeys);
	// Ustala funkcję zwrotną odpowiedzialną za badanie ruchu myszy

	glutMainLoop();
}
