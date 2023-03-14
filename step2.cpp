#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

#define WIRE 1
#define SHADE 0

typedef struct {
	float x, y, z;
}Point;

typedef struct {
	unsigned int ip[3];
}Face;


float lx = 0.0f, ly = 0.0f, lz = -1.0f;
float dx = 0.0f, dy = 1.75f, dz = 5.0f, ratio;
int pnum;
int fnum;
Point* mpoint = NULL;
Face* mface = NULL;
float angle = 0;
int status = 0;
int isCull = 0;

string fname = "myModel0.dat";
string fn_li[7] = {"myModel0.dat", "myModel4.dat", "myModel3.dat", "myModel1.dat",
"myModel2.dat", "myModel5.dat", "myModel6.dat" };

void InitLight() {
	float mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float mat_shininess[] = { 15.0 };

	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	float light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	float light_position[] = { 200, 200, -200.0, 0.0 };

	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

	glEnable(GL_COLOR_MATERIAL); 
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); 
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

Point cnormal(Point a, Point b, Point c) {
	Point p, q, r;
	double val;
	p.x = a.x - b.x; p.y = a.y - b.y; p.z = a.z - b.z;
	q.x = c.x - b.x; q.y = c.y - b.y; q.z = c.z - b.z;
	r.x = p.y * q.z - p.z * q.y; r.y = p.z * q.x - p.x * q.z; r.z = p.x * q.y - p.y * q.x;

	val = sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
	r.x = r.x / val; r.y = r.y / val; r.z = r.z / val;
	return r;
}


void ReadModel(string fname) {
	FILE* f1; char s[81]; int i;
	if (mpoint != NULL) delete mpoint;
	if (mface != NULL) delete mface;

	if ((f1 = fopen(fname.c_str(), "rt")) == NULL) { printf("No file\n"); exit(0); }
	fscanf(f1, "%s", s); fscanf(f1, "%s", s); 
	fscanf(f1, "%d", &pnum); 
	mpoint = new Point[pnum];

	for (i = 0; i < pnum; i++) {
		fscanf(f1, "%f", &mpoint[i].x); fscanf(f1, "%f", &mpoint[i].y); fscanf(f1, "%f", &mpoint[i].z);
	}

	fscanf(f1, "%s", s);fscanf(f1, "%s", s);fscanf(f1, "%d", &fnum); 

	mface = new Face[fnum];
	for (i = 0; i < fnum; i++) {
		fscanf(f1, "%d", &mface[i].ip[0]); fscanf(f1, "%d", &mface[i].ip[1]); fscanf(f1, "%d", &mface[i].ip[2]);
	}
	fclose(f1);
}

void makeMyMode(int i) {
	glPushMatrix();
	glRotatef(angle, 0.0, 1.0, 0.0);
	string fn = fn_li[i];
	ReadModel(fn);

	for (int i = 0; i < fnum; i++) {
		Point norm = cnormal(mpoint[mface[i].ip[2]], mpoint[mface[i].ip[1]], mpoint[mface[i].ip[0]]);
		glBegin(GL_TRIANGLES);
		glNormal3f(norm.x, norm.y, norm.z);
		glVertex3f(mpoint[mface[i].ip[0]].x, mpoint[mface[i].ip[0]].y, mpoint[mface[i].ip[0]].z);
		glNormal3f(norm.x, norm.y, norm.z);
		glVertex3f(mpoint[mface[i].ip[1]].x, mpoint[mface[i].ip[1]].y, mpoint[mface[i].ip[1]].z);
		glNormal3f(norm.x, norm.y, norm.z);
		glVertex3f(mpoint[mface[i].ip[2]].x, mpoint[mface[i].ip[2]].y, mpoint[mface[i].ip[2]].z);
		glEnd();
	}
	glPopMatrix();
}

void DrawWire(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (isCull) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
	}
	else glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glCallList(1);
	glutSwapBuffers();
}

void DrawShade(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (isCull) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
	}
	else glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCallList(1);
	glutSwapBuffers();
}

void MakeGL_Model(void) {
	int i;
	glShadeModel(GL_SMOOTH);

	if (glIsList(1))glDeleteLists(1, 1);
	glNewList(1, GL_COMPILE);

	glPushMatrix();
	glColor3f(0.3, 1, 1);
	glScalef(2, 2, 2);
	glRotatef(-30, 0, 1,0);
	makeMyMode(0);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1, 0.8, 0.4);
	glTranslatef(-7, 0, -5);
	glScalef(2, 2, 2);
	makeMyMode(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.2, 1);
	glTranslatef(10, 1, -5);
	glRotatef(-30, 0, 1, 0);
	makeMyMode(2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.4, 0, 0.1);
	glTranslatef(10, 1, 5);
	glScalef(2, 2, 2);
	glRotatef(-90, 0, 1, 0);
	makeMyMode(3);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 0.1);
	glTranslatef(-15, 1, 0);
	glScalef(3, 3, 3);
	makeMyMode(4);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.6, 1, 0.8);
	glTranslatef(15, 1, 10);
	glScalef(2, 2, 2);
	glRotatef(-90, 0, 1, 0);
	makeMyMode(5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0.5);
	glScalef(2, 2, 2);
	glTranslatef(-15, 0.7, 6);
	glRotatef(-90, 0, 1, 0);
	makeMyMode(6);
	glPopMatrix();

	glPushMatrix(); //바닥
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);
	glVertex3f(100, 0, 100);
	glVertex3f(-100, 0, 100);
	glVertex3f(-100, 0, -100);
	glVertex3f(100, 0, -100);
	glEnd();
	glPopMatrix();

	glPushMatrix();  //티팟
	glColor3f(0.8,	0.6, 1);
	glTranslatef(3, 1, -5);
	glutSolidTeapot(1);
	glPopMatrix();

	for (int j = 0; j < 2; j++) { // 벽 두개 그리기
		for (int i = 0; i < 20; i++) {
			if (i % 2 == 0) {
				glPushMatrix();
				glColor3f(1,0.3,0.3);
				glTranslatef(-100 + i * 10, 3, 50*pow(-1,j));
				glutSolidCube(10);
				glPopMatrix();
			}
			else {
				glPushMatrix();
				glColor3f(0,0,0);
				glTranslatef(-100 + i * 10, 3, 50*pow(-1,j));
				glutSolidCube(10);
				glPopMatrix();
			}
		}
	}
	
	glPushMatrix();  //공간 큐브
	glColor3f(0.7, 0.7, 1);
	glTranslatef(0, 0.5, 0);
	glutSolidCube(100);
	glPopMatrix();

	glPushMatrix();  // 구
	glColor3f(0.980392,	0.501961,	0.447059);
	glTranslatef(-5, 1, 13);
	glutSolidSphere(1, 20,  50);
	glPopMatrix();
	glEndList();
}

void GLSetupRC(void) {
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.5, 0.5, 0.5);
	MakeGL_Model();
	if (status == WIRE)DrawWire();
	else DrawShade();
}

void rotate_leftnright(float ang)
{
		lx = sin(ang);
		lz = -cos(ang);
		glLoadIdentity();
		gluLookAt(dx, dy, dz,
			dx + lx, dy + ly, dz + lz,
			0.0f, 1.0f, 0.0f);
}


void go_frontnback(int direction)
{
	dx = dx + direction * (lx) * 0.1;
	dz = dz + direction * (lz) * 0.1;
	glLoadIdentity();
	gluLookAt(dx, dy, dz,
		dx + lx, dy + ly, dz + lz,
		0.0f, 1.0f, 0.0f);
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'z':status = WIRE, glutPostRedisplay(); break; // wire
	case 'x':status = SHADE; glutPostRedisplay(); break; // shade
	case 'c':InitLight(); break; // light on
	case 'v': glDisable(GL_LIGHTING); glutPostRedisplay(); break; //light off
	case '.':if (isCull)isCull = 0; //culling off
			else isCull = 1;
		glutPostRedisplay(); break;//culling on
	case 'a': //ad로 좌우 회전
		angle -= 0.02f;
		rotate_leftnright(angle);
		break;
	case 'd':
		angle += 0.02f;
		rotate_leftnright(angle);
		break;
	case 'w': //ws로 앞뒤 이동
		go_frontnback(1);
		break;
	case 's':
		go_frontnback(-1);
		break;
	}
}

void myreshape(int w, int h)
{
	ratio = 1.0f * w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(dx, dy, dz,
		dx + lx, dy + ly, dz + lz,
		0.0f, 1.0f, 0.0f);
}

void Myhelp()
{
	printf("Help >> Mouse Button ^0^ \n");

	printf("w ==> Go forward \n");
	printf("s ==> Go backward \n");
	printf("a ==> Rotate left \n");
	printf("d ==> Rotate right \n");
	printf("z ==> Wire \n");
	printf("x ==> Shade \n");
	printf("c ==> Light On \n");
	printf("v ==> Light Off\n");
	printf(". ==> Culling On/Off\n");
	printf("즐거운 관람 되시길 * *(_ ^ _) * *");
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(350, 100);

	glutCreateWindow("CG2021_Final_문유빈");

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
	ReadModel(fname);
	GLSetupRC();
	glutReshapeFunc(myreshape);
	Myhelp();
	glutMainLoop();
	return 0;
}