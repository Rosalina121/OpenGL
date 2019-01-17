#include <windows.h>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>
#include <math.h>

using namespace std;
typedef float point3[3];

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
// inicjalizacja położenia obserwatora

static GLfloat thetaArr[] = { 0.0, 0.0, 0.0 }; // trzy kšty obrotu
static GLfloat theta = 0.0;   // kšt obrotu obiektu
static GLfloat alfa = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy
							   // 0 - nie nacinięto żadnego klawisza
							   // 1 - nacinięty zostać lewy klawisz
							   // 2 - wcisniety prawy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int scroll_old = 0;
static int delta_x = 0;        // różnica pomiędzy pozycjš bieżšcš
static int delta_y = 0;
static int delta_scroll = 0;

const int n = 80;
int model = 1;

double tabTekstury[n][n][2];

bool wall1 = true;
bool wall2 = true;
bool wall3 = true;
bool wall4 = true;
bool wall5 = true;

void RenderScene(void);

// i poprzedniš kursora myszy

/*************************************************************************************/

void spinEgg()
{

	thetaArr[0] -= 0.5;
	if (thetaArr[0] > 360.0) thetaArr[0] -= 360.0;

	thetaArr[1] -= 0.5;
	if (thetaArr[1] > 360.0) thetaArr[1] -= 360.0;

	thetaArr[2] -= 0.5;
	if (thetaArr[2] > 360.0) thetaArr[2] -= 360.0;

	glutPostRedisplay(); //odwieżenie zawartoci aktualnego okna
}



/*************************************************************************************/
void Egg() {
	point3 **vectors3d = new point3*[n];
	point3 **vektorNorm = new point3*[n];
	for (int i = 0; i < n; i++) {
		vectors3d[i] = new point3[n];
		vektorNorm[i] = new point3[n];
	}


	double odstep = 1 / (double)(n - 1);
	double ***punkty2D = new double**[n];
	for (int i = 0; i < n; i++) {
		punkty2D[i] = new double*[n];
		for (int j = 0; j < n; j++) {
			punkty2D[i][j] = new double[2];
			punkty2D[i][j][0] = i * odstep;
			punkty2D[i][j][1] = j * odstep;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			double u = punkty2D[i][j][0];
			double v = punkty2D[i][j][1];

			tabTekstury[i][j][0] = u;
			tabTekstury[i][j][1] = v;

			vectors3d[i][j][0] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(3.14 *v);
			vectors3d[i][j][1] = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2) - 5;
			vectors3d[i][j][2] = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(3.14 *v);

			float ux, uz, uy, vz, vy, vx, length;

			// obliczenie wartosci wektorow normalnych
			ux = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(3.14*v);
			uy = (640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u);
			uz = (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(3.14*v);

			vx = 3.14*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(3.14*v);
			vy = 0;
			vz = -3.14*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(3.14*v);

			vektorNorm[i][j][0] = uy * vz - uz * vy;
			vektorNorm[i][j][1] = uz * vx - ux * vz;
			vektorNorm[i][j][2] = ux * vy - uy * vx;

			length = sqrt(vektorNorm[i][j][0] * vektorNorm[i][j][0] + vektorNorm[i][j][1] * vektorNorm[i][j][1] + vektorNorm[i][j][2] * vektorNorm[i][j][2]);
			if (i == 0 || i == n)
			{
				vektorNorm[i][j][0] = 0;
				vektorNorm[i][j][1] = -1;
				vektorNorm[i][j][2] = 0;
			}
			else if (i < n / 2)
			{
				vektorNorm[i][j][0] = (uy*vz - uz * vy) / length;
				vektorNorm[i][j][1] = (uz*vx - ux * vz) / length;
				vektorNorm[i][j][2] = (ux*vy - uy * vx) / length;
			}
			else if (i > n / 2)
			{
				vektorNorm[i][j][0] = -1 * (uy*vz - uz * vy) / length;
				vektorNorm[i][j][1] = -1 * (uz*vx - ux * vz) / length;
				vektorNorm[i][j][2] = -1 * (ux*vy - uy * vx) / length;
			}
			else
			{
				vektorNorm[i][j][0] = 0;
				vektorNorm[i][j][1] = 1;
				vektorNorm[i][j][2] = 0;
			}


		}
	}
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (i < n / 2)
			{
				glBegin(GL_TRIANGLES);
				glNormal3fv(vektorNorm[i][j + 1]);
				glTexCoord2f(tabTekstury[i][j + 1][0], tabTekstury[i][j + 1][1]);
				glVertex3fv(vectors3d[i][j + 1]);

				glNormal3fv(vektorNorm[i + 1][j]);
				glTexCoord2f(tabTekstury[i + 1][j][0], tabTekstury[i + 1][j][1]);
				glVertex3fv(vectors3d[i + 1][j]);

				glNormal3fv(vektorNorm[i + 1][j + 1]);
				glTexCoord2f(tabTekstury[i + 1][j + 1][0], tabTekstury[i + 1][j + 1][1]);
				glVertex3fv(vectors3d[i + 1][j + 1]);
				glEnd();


				glBegin(GL_TRIANGLES);
				glNormal3fv(vektorNorm[i][j]);
				glTexCoord2f(tabTekstury[i][j][0], tabTekstury[i][j][1]);
				glVertex3fv(vectors3d[i][j]);

				glNormal3fv(vektorNorm[i + 1][j]);
				glTexCoord2f(tabTekstury[i + 1][j][0], tabTekstury[i + 1][j][1]);
				glVertex3fv(vectors3d[i + 1][j]);

				glNormal3fv(vektorNorm[i][j + 1]);
				glTexCoord2f(tabTekstury[i][j + 1][0], tabTekstury[i][j + 1][1]);
				glVertex3fv(vectors3d[i][j + 1]);
				glEnd();
			}
			else
			{
				glBegin(GL_TRIANGLES);
				glNormal3fv(vektorNorm[i + 1][j + 1]);
				glTexCoord2f(tabTekstury[i + 1][j + 1][0], tabTekstury[i + 1][j + 1][1]);
				glVertex3fv(vectors3d[i + 1][j + 1]);



				glNormal3fv(vektorNorm[i + 1][j]);
				glTexCoord2f(tabTekstury[i + 1][j][0], tabTekstury[i + 1][j][1]);
				glVertex3fv(vectors3d[i + 1][j]);

				glNormal3fv(vektorNorm[i][j + 1]);
				glTexCoord2f(tabTekstury[i][j + 1][0], tabTekstury[i][j + 1][1]);
				glVertex3fv(vectors3d[i][j + 1]);

				glEnd();


				glBegin(GL_TRIANGLES);
				glNormal3fv(vektorNorm[i][j + 1]);
				glTexCoord2f(tabTekstury[i][j + 1][0], tabTekstury[i][j + 1][1]);
				glVertex3fv(vectors3d[i][j + 1]);

				glNormal3fv(vektorNorm[i + 1][j]);
				glTexCoord2f(tabTekstury[i + 1][j][0], tabTekstury[i + 1][j][1]);
				glVertex3fv(vectors3d[i + 1][j]);

				glNormal3fv(vektorNorm[i][j]);
				glTexCoord2f(tabTekstury[i][j][0], tabTekstury[i][j][1]);
				glVertex3fv(vectors3d[i][j]);

				glEnd();
			}




		}
	}
}

void Pyramid()
{
	glBegin(GL_TRIANGLES);

	if (wall1)
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
	}

	if (wall2)
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
	}

	if (wall3)
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
	}

	if (wall4)
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
	}


	glEnd();

	if (wall5)
	{
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);


		glEnd();
	}
}

void Triangle()
{
	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glEnd();
}

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora
		y_pos_old = y;
		// jako pozycji poprzedniej
		status = 1;          // wcięnięty został lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		scroll_old = y;
		// jako pozycji poprzedniej
		status = 2;          // wcięnięty został prawy klawisz myszy
	}
	else
		status = 0;          // nie został wcięnięty żaden klawisz
}



void keys(unsigned char key, int x, int y)
{
	if (key == '1') model = 1;
	if (key == '2') model = 2;
	if (key == '3') model = 3;
	if (key == 'q') wall1 = !wall1;
	if (key == 'w') wall2 = !wall2;
	if (key == 'e') wall3 = !wall3;
	if (key == 'r') wall4 = !wall4;
	if (key == 't') wall5 = !wall5;

	RenderScene(); // przerysowanie obrazu sceny
}
/*************************************************************************************/

// Funkcja "monitoruje" położenie kursora myszy i ustawia wartoci odpowiednich
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{
	if (status == 1) {
		delta_x = x - x_pos_old;     // obliczenie różnicy położenia kursora myszy
		delta_y = y - y_pos_old;
		x_pos_old = x;            // podstawienie bieżšcego położenia jako poprzednie
		y_pos_old = y;
	}
	else if (status == 2) {
		delta_scroll = y - scroll_old;
		scroll_old = y;
	}
	glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/


// Funkcja rysujšca osie układu wspó?rz?dnych



void Axes(void)
{

	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	//  pocz?tek i koniec obrazu osi y

	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x

	glVertex3fv(x_min);
	glVertex3fv(x_max);

	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);

	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);

	glEnd();

}

/*************************************************************************************/

// Funkcja okrelajšca co ma być rysowane (zawsze wywoływana, gdy trzeba
// przerysować scenę)



void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczšcym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej
	float scroll = delta_scroll * pix2angle*0.07;
	if (status == 2 && ((viewer[2] <= 6 && scroll > 0) || (scroll < 0 && viewer[2] >= 20) || (viewer[2] > 6 && viewer[2] < 20))) {

		viewer[2] += scroll;
	}
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
	// Zdefiniowanie położenia obserwatora


	if (status == 1)                     // jeli lewy klawisz myszy wcięnięty
	{
		theta += delta_x * pix2angle;    // modyfikacja kšta obrotu o kat proporcjonalny
		alfa += delta_y * pix2angle;

	}

	glRotatef(thetaArr[0], 1.0, 0.0, 0.0);

	glRotatef(thetaArr[1], 0.0, 1.0, 0.0);

	glRotatef(thetaArr[2], 0.0, 0.0, 1.0);
	glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kšt
	glRotatef(alfa, 1.0, 0.0, 0.0);  //obrót obiektu o nowy kšt



	glColor3f(1.0f, 23.0f, 1.0f);
	// Ustawienie koloru rysowania na biały
	switch (model) {
	case 1:
		Egg();
		break;
	case 2:
		Pyramid();
		break;
	case 3:
		Triangle();
		break;
	}
	// Narysowanie czajnika

	Axes();

	glFlush();
	// Przekazanie poleceń rysujšcych do wykonania

	glutSwapBuffers();



}
/*************************************************************************************/

// Funkcja ustalajšca stan renderowania

/*************************************************************************************/
 // Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie
 // FileName, alokuje pamięć i zwraca wskanik (pBits) do bufora w którym
 // umieszczone sš dane.
 // Ponadto udostępnia szerokoć (ImWidth), wysokoć (ImHeight) obrazu
 // tekstury oraz dane opisujšce format obrazu według specyfikacji OpenGL
 // (ImComponents) i (ImFormat).
 // Jest to bardzo uproszczona wersja funkcji wczytujšcej dane z pliku TGA.
 // Działa tylko dla obrazów wykorzystujšcych 8, 24, or 32 bitowy kolor.
 // Nie obsługuje plików w formacie TGA kodowanych z kompresjš RLE.
/*************************************************************************************/


GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{

	/*************************************************************************************/

	// Struktura dla nagłówka pliku  TGA


#pragma pack(1)           
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)

	FILE *pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte    *pbitsperpixel = NULL;


	/*************************************************************************************/

	// Wartoci domylne zwracane w przypadku błędu

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;

	/*************************************************************************************/
	// Przeczytanie nagłówka pliku 


	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);


	/*************************************************************************************/

	// Odczytanie szerokoci, wysokoci i głębi obrazu

	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;


	/*************************************************************************************/
	// Sprawdzenie, czy głębia spełnia założone warunki (8, 24, lub 32 bity)

	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	/*************************************************************************************/

	// Obliczenie rozmiaru bufora w pamięci


	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;


	/*************************************************************************************/

	// Alokacja pamięci dla danych obrazu


	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}


	/*************************************************************************************/

	// Ustawienie formatu OpenGL


	switch (sDepth)

	{

	case 3:

		*ImFormat = GL_BGR_EXT;

		*ImComponents = GL_RGB8;

		break;

	case 4:

		*ImFormat = GL_BGRA_EXT;

		*ImComponents = GL_RGBA8;

		break;

	case 1:

		*ImFormat = GL_LUMINANCE;

		*ImComponents = GL_LUMINANCE8;

		break;

	};



	fclose(pFile);



	return pbitsperpixel;

}

/*************************************************************************************/

void MyInit(void)
{

	/*************************************************************************************/

// Zmienne dla obrazu tekstury



	GLbyte *pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat mat_ambient[] = { 1.0,1.0, 1.0, 1 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 20.0 };
	GLfloat light_position[] = { 0.0, 0.0, 30.0, 1.0 };
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat att_constant = { 1.0 };
	GLfloat att_linear = { (GLfloat) 0.05 };
	GLfloat att_quadratic = { (GLfloat) 0.001 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	glShadeModel(GL_SMOOTH); // właczenie łagodnego cieniowania
	glEnable(GL_LIGHTING);   // właczenie systemu owietlenia sceny
	glEnable(GL_LIGHT0);     // włšczenie ródła o numerze 0
	glEnable(GL_DEPTH_TEST); // włšczenie mechanizmu z-bufora

	/*************************************************************************************/

// Teksturowanie będzie prowadzone tyko po jednej stronie ciany

	glEnable(GL_CULL_FACE);

	/*************************************************************************************/

	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga

	pBytes = LoadTGAImage("palitechnika1024.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	/*************************************************************************************/

   // Zdefiniowanie tekstury 2-D

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	/*************************************************************************************/

	// Zwolnienie pamięci

	free(pBytes);

	/*************************************************************************************/

	// Włšczenie mechanizmu teksturowania

	glEnable(GL_TEXTURE_2D);

	/*************************************************************************************/

	// Ustalenie trybu teksturowania

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*************************************************************************************/

	// Okrelenie sposobu nakładania tekstur

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoć i szerokoć okna) sš
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;

	glMatrixMode(GL_PROJECTION);
	// Przełšczenie macierzy bieżšcej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bieżšcej
	cout << vertical << " horizontal " << horizontal << endl;

	GLdouble aspect = (GLdouble)horizontal / vertical;

	gluPerspective(70, (float)aspect, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoci okna okna widoku (viewport) w zależnoci
	// relacji pomiędzy wysokociš i szerokociš okna

	glMatrixMode(GL_MODELVIEW);
	// Przełšczenie macierzy bieżšcej na macierz widoku modelu 

	glLoadIdentity();
	// Czyszczenie macierzy bieżšcej

}

/*************************************************************************************/

// Główny punkt wejcia programu. Program działa w trybie konsoli



void main(void)
{
	char *myargv[1];
	int myargc = 1;
	myargv[0] = _strdup("");
	glutInit(&myargc, myargv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1000, 1000);

	glutCreateWindow("Teksturki");

	//glutIdleFunc(spinEgg);

	glutDisplayFunc(RenderScene);
	// Okrelenie, że funkcja RenderScene będzie funkcjš zwrotnš
	// (callback function).  Będzie ona wywoływana za każdym razem
	// gdy zajdzie potrzeba przerysowania okna


	glutMouseFunc(Mouse);

	glutKeyboardFunc(keys);

	glutMotionFunc(Motion);

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcję zwrotnš odpowiedzialnš
	// za zmiany rozmiaru okna                      


	MyInit();
	// Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przystšpieniem do renderowania

	glEnable(GL_DEPTH_TEST);
	// Włšczenie mechanizmu usuwania niewidocznych elementów sceny

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}
