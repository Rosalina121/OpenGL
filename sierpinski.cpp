#include "pch.h"
#include <windows.h>
#include <time.h>
#include <gl/gl.h>
#include <glut.h>

GLint poziom = 3;
GLfloat szerokosc = 1000;
GLfloat st_deformacji = 2;

void rysuj_dywan(GLfloat x, GLfloat y, GLfloat szerokosc, GLint poziom) {
	if (poziom > 0) {
		szerokosc = szerokosc/3;
		rysuj_dywan(x, y, szerokosc, poziom-1);
		rysuj_dywan(x-szerokosc, y, szerokosc, poziom-1);
		rysuj_dywan(x-2*szerokosc, y, szerokosc, poziom-1);
        rysuj_dywan(x,y-szerokosc, szerokosc, poziom-1);
        rysuj_dywan(x-2*szerokosc, y-szerokosc, szerokosc, poziom-1);
        rysuj_dywan(x, y-2*szerokosc, szerokosc, poziom-1);
        rysuj_dywan(x-szerokosc, y-2*szerokosc, szerokosc, poziom-1);
        rysuj_dywan(x-2*szerokosc, y-2*szerokosc, szerokosc, poziom-1);
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

    glColor3f(0.0f, 1.0f, 0.0f);
 
	rysuj_dywan(50,50,szerokosc,poziom);

    glFlush();
}

void MyInit(void) {
   glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

}

void ChangeSize(GLsizei horizontal, GLsizei vertical) {
     GLfloat AspectRatio;

    if (vertical==0)
        vertical = 1;

    glViewport(0, 0, horizontal, vertical);

    glMatrixMode(GL_PROJECTION); 

    glLoadIdentity();

    AspectRatio = (GLfloat)horizontal/(GLfloat)vertical;

    if (horizontal <= vertical)
		glOrtho(-100.0,100.0,-100.0/AspectRatio,100.0/AspectRatio,1.0,-1.0);  
	else
		glOrtho(-100.0*AspectRatio,100.0*AspectRatio,-100.0,100.0,1.0,-1.0);

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
