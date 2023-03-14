#include <GL/glut.h>
#include <GL/freeglut.h>
#include <vector>
#include <iostream>
#include <math.h>
# define M_PI 3.14159265358979323846

float winWidth = 650, winHeight = 500;

class xPoint3D {
public:
	float x, y, z, w;
	xPoint3D() { x = y = z = 0; w = 1; };
};

std::vector<xPoint3D> arPoints;
std::vector<xPoint3D> arRotPoints;

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);  // 검은 배경
	glClear(GL_COLOR_BUFFER_BIT);
}

void drawXYLine() {
	glColor3f(1.0, 1.0, 1.0);		// 축 하얀 색

	glBegin(GL_LINES);		// x축 그리기
	glVertex3f(-1.0, 0, 0);
	glVertex3f(1, 0, 0);
	glEnd();

	glBegin(GL_LINES);		// y 축 그리기
	glVertex3f(0, 1.0, 0);
	glVertex3f(0, -1.0, 0);
	glEnd();
	glFlush();
}

void Mydisplay() {
	drawXYLine();		// 축 그리기
	glColor3f(1, 0.843137, 0);		// 점 gold 색상
	glPointSize(4.0);
}

void MyReshape(int NewWidth, int NewHeight) {
	glViewport(0, 0, NewWidth, NewHeight);
	float WidthFactor = (float)NewWidth / (float)winWidth;
	float HeightFactor = (float)NewHeight / (float)winHeight;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0 * WidthFactor, 1.0 * WidthFactor, -1.0 * HeightFactor, 1.0 * HeightFactor, -1.0, 1.0);
}

void drawPt(xPoint3D pt) {		// 점 찍기
	glBegin(GL_POINTS);
	glVertex3f(pt.x, pt.y, pt.z);
	glEnd();
	glFlush();
}

void getPt(int button, int action, int xMouse, int yMouse) {
	xPoint3D pt;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {
		pt.x = -1 + xMouse / 325.5;	pt.y = ((winHeight - yMouse) / 250) - 1;	pt.z = 0;
		drawPt(pt);
		arPoints.push_back(pt);
	}
	else
		if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) exit(0);
}

void drawLine(int fRotAngle) {
	int rot_numb = 360 / fRotAngle;
	glColor3f(0.5, 0.5, 0.5);
	for (int i = 1; i < arRotPoints.size() - rot_numb; i++) {
		if (i % (rot_numb + 1) == 0) {}
		else {
			if (i % (rot_numb + 1) == rot_numb) {
				glBegin(GL_LINE_LOOP);
				glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
				glVertex3f(arRotPoints[i + rot_numb].x, arRotPoints[i + rot_numb].y, arRotPoints[i + rot_numb].z);
				glVertex3f(arRotPoints[i + rot_numb + 1].x, arRotPoints[i + rot_numb + 1].y, arRotPoints[i + rot_numb + 1].z);
				glEnd();
				glBegin(GL_LINE_LOOP);
				glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
				glVertex3f(arRotPoints[i + 1].x, arRotPoints[i + 1].y, arRotPoints[i + 1].z);
				glVertex3f(arRotPoints[i - rot_numb + 1].x, arRotPoints[i - rot_numb + 1].y, arRotPoints[i - rot_numb + 1].z);
				glEnd();
			}
			else {
				glBegin(GL_LINE_LOOP);
				glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
				glVertex3f(arRotPoints[i + rot_numb].x, arRotPoints[i + rot_numb].y, arRotPoints[i + rot_numb].z);
				glVertex3f(arRotPoints[i + rot_numb + 1].x, arRotPoints[i + rot_numb + 1].y, arRotPoints[i + rot_numb + 1].z);
				glEnd();
				glBegin(GL_LINE_LOOP);
				glVertex3f(arRotPoints[i].x, arRotPoints[i].y, arRotPoints[i].z);
				glVertex3f(arRotPoints[i + 1].x, arRotPoints[i + 1].y, arRotPoints[i + 1].z);
				glVertex3f(arRotPoints[i + rot_numb + 1].x, arRotPoints[i + rot_numb + 1].y, arRotPoints[i + rot_numb + 1].z);
				glEnd();
			}
		}
	}
}

void rotatePtY(float fRotAngle) {
	float radian = fRotAngle * (M_PI / 180);
	xPoint3D newPt;
	int rot_numb = 360 / fRotAngle;
	for (int i = 0; i < arPoints.size(); i++) {
		for (int j = 0; j <= rot_numb; j++) {
			newPt.x = arPoints[i].z * sin(radian * j) + arPoints[i].x * cos(radian * j);
			newPt.y = arPoints[i].y;
			newPt.z = arPoints[i].z * cos(radian * j) - arPoints[i].x * sin(radian * j);
			arRotPoints.push_back(newPt);
			drawPt(newPt);
		}
	}
	drawLine(fRotAngle);
	glFlush();
}

void rotatePtX(float fRotAngle) {
	float radian = fRotAngle * (M_PI / 180);
	xPoint3D newPt;
	int rot_numb = 360 / fRotAngle;
	for (int i = 0; i < arPoints.size(); i++) {
		for (int j = 0; j <= rot_numb; j++) {
			newPt.x = arPoints[i].x;
			newPt.y = arPoints[i].y * cos(radian * j) - arPoints[i].z * sin(radian * j);
			newPt.z = arPoints[i].y * sin(radian * j) + arPoints[i].z * cos(radian * j);
			arRotPoints.push_back(newPt);
			drawPt(newPt);
		}
	}
	drawLine(fRotAngle);
	glFlush();
}

void MyMainMenu(int entryID) {
	if (entryID == 1) {}
	else if (entryID == 2) {}
	else if (entryID == 3) {
		arPoints.clear();
		arRotPoints.clear();
		init();
		glutDisplayFunc(Mydisplay);
	}
	else if (entryID == 4) exit(0);
	glutPostRedisplay();
}

void MySubMenuY(int entryID) {
	if (entryID == 1)	rotatePtY(30);
	else if (entryID == 2) rotatePtY(60);
	else if (entryID == 3)	rotatePtY(90);
	glutPostRedisplay();
}

void MySubMenuX(int entryID) {
	if (entryID == 1)	rotatePtX(30);
	else if (entryID == 2) rotatePtX(60);
	else if (entryID == 3)	rotatePtX(90);
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Midexam_20193852 문유빈");

	init();
	glutDisplayFunc(Mydisplay);

	int MySubMenuIDY = glutCreateMenu(MySubMenuY);
	glutAddMenuEntry("30", 1);
	glutAddMenuEntry("60", 2);
	glutAddMenuEntry("90", 3);

	int MySubMenuIDX = glutCreateMenu(MySubMenuX);
	glutAddMenuEntry("30", 1);
	glutAddMenuEntry("60", 2);
	glutAddMenuEntry("90", 3);

	int MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddSubMenu("Rotate by X", MySubMenuIDX);
	glutAddSubMenu("Rotate by Y", MySubMenuIDY);
	glutAddMenuEntry("Reset", 3);
	glutAddMenuEntry("Exit", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutReshapeFunc(MyReshape);
	glutMouseFunc(getPt);
	glutMainLoop();
}