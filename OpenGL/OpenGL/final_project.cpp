#include <iostream>
using namespace std;

#include<glut.h>
#include<gl.h>
#include<glu.h>

static int Day = 0, Time = 0, Day2 = 0, Day3 = 0;

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 1.0, 1.5, 0.0);  // 전체 모델을 기울임
	glColor3f(1.0, 0.3, 0.3); // 태양의 색
	glutWireSphere(0.13, 20, 16); // 태양을 그려냄
	glPushMatrix(); // 태양기준의 좌표계를 스택에 푸시하여 저장
	glRotatef((GLfloat)Day, 0.0, 1.0, 0.0);
	glTranslatef(0.45, 0.0, 0.0); // 태양으로부터 0.45만큼 떨어진 거리에위치
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0); //
	glColor3f(0.5, 0.6, 0.7);
	glutWireSphere(0.06, 10, 8); // 지구를 그려냄
	glPushMatrix(); // 지구기준의 좌표계를 스택에 푸시하여 저장
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glTranslatef(0.12, 0.0, 0.0); // 지구로부터 0.12 떨어진 거리에 위치
	glColor3f(0.9, 0.8, 0.2);  // 달의 색 지정
	glutWireSphere(0.015, 10, 8); // 달을 그려냄
	glPopMatrix();
	glPopMatrix();
	glLoadIdentity(); // 전역 좌표 기준 초기화
	gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 1.0, 1.03, 0.0); // 태양계 행성들이 거희 유사하게 기울어져있다고
															 //만 되어 있기에 약간의 기울기 차이만 줌
	glRotatef((GLfloat)Day2, 0.0, 1.0, 0.0);
	glTranslatef(0.65, 0.0, 0.0); // 태양으로부터 0.65 떨어진 거리에 위치
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glColor3f(0.9, 0.8, 0.1); // 금성의 색 지정
	glutWireSphere(0.05, 10, 8); // 금성을 그려냄
	glLoadIdentity();
	gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 1.0, 1.025, 0.0);// 태양계 행성들이 거희 유사하게 기울어져있다고
															 //만 되어 있기에 약간의 기울기 차이만 줌
	glRotatef((GLfloat)Day3, 0.0, 1.0, 0.0);
	glTranslatef(0.95, 0.0, 0.0); // 태양으로부터 0.95 떨어진 거리에 위치
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 1.0); // 수성의 색 지정
	glutWireSphere(0.026, 10, 8); // 수성을 그려냄
	glutSwapBuffers();
}

void MyKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'd':
		Day = (Day + 1) % 360;
		glutPostRedisplay();
		Time = (Time + 10) % 360;
		glutPostRedisplay();
		Day2 = (Day2 + 2) % 360;  // 금성의 공전주기는 227일 지구가 0.6 회전시 1번 완전한 회전을 하지만
								  // 소수점 단위의 표현이 난해하여 지구 1번 회전시 2번회전을 시킴
		glutPostRedisplay();
		Day3 = (Day3 + 4) % 360;  // 수성의 공전주기는 87일 지구기 1번 회전시 4번 회전
		glutPostRedisplay();

		break;
	default:
		break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGl Sample Drawing");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutMainLoop();
	return 0;
}