#include <windows.h>
#include <time.h>
#include <gl/gl.h>

#include <gl/glut.h>


GLint poziom = 3;
GLfloat szerokosc = 100;
GLfloat st_deformacji = 3;

void rysuj_dywan( GLfloat x, GLfloat y, GLfloat szerokosc, GLint poziom) {
	
	if (poziom > 0) {
		szerokosc = szerokosc/3;
		rysuj_dywan(x,y,szerokosc,poziom-1);
		rysuj_dywan(x-szerokosc,y,szerokosc,poziom-1);
		rysuj_dywan(x-2*szerokosc,y,szerokosc,poziom-1);
        rysuj_dywan(x,y-szerokosc,szerokosc,poziom-1);
        rysuj_dywan(x-2*szerokosc,y-szerokosc,szerokosc,poziom-1);
        rysuj_dywan(x,y-2*szerokosc,szerokosc,poziom-1);
        rysuj_dywan(x-szerokosc,y-2*szerokosc,szerokosc,poziom-1);
        rysuj_dywan(x-2*szerokosc,y-2*szerokosc,szerokosc,poziom-1);
	}

	else {
		GLfloat def = (rand() % 20)*st_deformacji /(10*szerokosc);
		glBegin(GL_POLYGON);                                                 
			glColor3f(((rand() %100)*0.01), ((rand() %100)*0.01), ((rand() %100)*0.01));
			glVertex2f(x+def, y+def);
			glColor3f(((rand() %100)*0.01), ((rand() %100)*0.01), ((rand() %100)*0.01));
			glVertex2f(x+def,y-szerokosc+def);
			glColor3f(((rand() %100)*0.01), ((rand() %100)*0.01), ((rand() %100)*0.01));
			glVertex2f(x-szerokosc+def, y-szerokosc+def);
			glColor3f(((rand() %100)*0.01), ((rand() %100)*0.01), ((rand() %100)*0.01));
			glVertex2f(x-szerokosc+def, y+def); 
		glEnd();
	}
}

void RenderScene(void) {

    glClear(GL_COLOR_BUFFER_BIT);
    // Czyszczenie okna aktualnym kolorem czyszcz�cym

 

    glColor3f(0.0f, 1.0f, 0.0f);
    // Ustawienie aktualnego koloru rysowania na zielony

	rysuj_dywan(50,50,szerokosc,poziom);

    glFlush();
    // Przekazanie polece� rysuj�cych do wykonania

}

void MyInit(void) {

   glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
   // Kolor okna wn�trza okna - ustawiono na szary

}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {

     GLfloat AspectRatio;

     // Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna

 

    if (vertical==0)
    // Zabezpieczenie pzred dzieleniem przez 0

        vertical = 1;

     glViewport(0, 0, horizontal, vertical);
     // Ustawienie wielko�ciokna okna urz�dzenia (Viewport)
     // W tym przypadku od (0,0) do (horizontal, vertical)

    glMatrixMode(GL_PROJECTION); 
    // Okre�lenie uk�adu wsp�rz�dnych obserwatora

    glLoadIdentity();
    // Okre�lenie przestrzeni ograniczaj�cej

    AspectRatio = (GLfloat)horizontal/(GLfloat)vertical;
    // Wyznaczenie wsp�czynnika proporcji okna

    // Gdy okno na ekranie nie jest kwadratem wymagane jest
    // okre�lenie okna obserwatora.
    // Pozwala to zachowa� w�a�ciwe proporcje rysowanego obiektu
    // Do okre�lenia okna obserwatora s�u�y funkcja glOrtho(...)


    if (horizontal <= vertical)

    glOrtho(-100.0,100.0,-100.0/AspectRatio,100.0/AspectRatio,1.0,-1.0);  

    else

    glOrtho(-100.0*AspectRatio,100.0*AspectRatio,-100.0,100.0,1.0,-1.0);

 

    glMatrixMode(GL_MODELVIEW);
    // Okre�lenie uk�adu wsp�rz�dnych    

    glLoadIdentity(); 

}

void main(void) {

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   // Ustawienie trybu wy�wietlania
   // GLUT_SINGLE - pojedynczy bufor wy�wietlania
   // GLUT_RGBA - model kolor�w RGB


   glutCreateWindow("Drugi program w OpenGL");
   // Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna

   glutDisplayFunc(RenderScene);
   // Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
   // Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
   // trzeba b�dzie przerysowa� okno


   glutReshapeFunc(ChangeSize);
   // Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
   // zmiany rozmiaru okna

   MyInit();
   // Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie 
   // inicjalizacje konieczneprzed przyst�pieniem do renderowania


   glutMainLoop();
   // Funkcja uruchamia szkielet biblioteki GLUT
}
