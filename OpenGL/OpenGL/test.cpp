#include <windows.h>
#include <glut.h>
#include <math.h>
#define Pi 3.14



void myDisplay_shadeModel() {
	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(GL_SMOOTH); // 그릴 다각형의 내부는 그라데이션으로 칠할 것
	//glShadeModel(GL_FLAT); // 단색 (가장 위의 점의 색깔만 => 즉, 가장 마지막 코드의 색상인 B!!)

	glBegin(GL_TRIANGLES); // 삼각형 그리기 시작
	glColor3f(0.0, 1.0, 0.0); // G
	glVertex3f(0.0, 0.5, 0.0);
	glColor3f(1.0, 0.0, 0.0); // R
	glVertex3f(-0.5, -0.5, 0.0);
	glColor3f(0.0, 0.0, 1.0); // B
	glVertex3f(0.5, -0.5, 0.0);
	glEnd();

	glFlush();
}


void myDisplay_line() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES); // 선그리기 -> 두 점 정의

	glLineWidth(5.0); // 선 두께
	glColor3f(1.0, 0.0, 0.0); // 빨강
	glVertex3f(1.0, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0); // 초록
	glVertex3f(-1.0, 0.0, 0.0);
	 
	glColor3f(0.0, 0.0, 1.0); // 파랑
	glVertex3f(0.5, 0.5, 0.0); // ***x값을 0.5로 바꾸면 창 가로 크기의 0.5배 위치에 점 있음***
	glColor3f(0.0, 1.0, 1.0); // 하늘색 (G+B)
	glVertex3f(-1.0, 0.5, 0.0);

	glEnd();

	glFlush(); 
}


void myDisplay_circle() {
	GLfloat size[2]; // 크기가 2짜리인 size 배열 생성. 자료형은 GLfloat type
	GLfloat angle; // 아래 for문에서 쓰기 위해 angle 변수 선언
	glClear(GL_COLOR_BUFFER_BIT);
	// 여기부터
	glColor3f(1.0, 1.0, 1.0); // 흰색
	glGetFloatv(GL_POINT_SIZE_RANGE, size); // HW가 지원가능한 점의 크기를 size라는 배열에 받음
	//glPointSize(size[1]); // size[0] : 그 중 가장 작은 크기의 값으로 그릴 점의 크기 지정 (size[1] : 가장 큰 크기 값)
	glPointSize((size[1] - size[0]) / 2.0 + size[0]); // 평균 크기의 점~
	glBegin(GL_POINTS); // 점들을 계속 지정 시작~
	
	// 0 radian부터 2파이 radian ==> 즉, 0~360도까지, 파이/30 = 180/30 = 6도 간격으로 점 찍기 => 정확히는 점 61개 찍힘 (마지막 겹쳐서)
	for (angle = 0.0; angle <= 2.0 * Pi; angle += Pi / 30.0) {
		glVertex3f(0.5 * cos(angle), 0.5 * sin(angle), 0.0);
	}
	glEnd(); // 없으면 그냥 흰 창. 아무것도 안그려짐
	// 여기까지 중요한 부분
	glFlush(); 
}

void myDisplay_teapot() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 0.0); //노란색 티팟
	glutSolidTeapot(0.5); // 주전자 내부
	glColor3f(1.0, 0.0, 0.0); // 빨간색
	glutWireTeapot(0.5); // 와이어 티팟!

	glFlush(); // 그래픽 프로그래밍한 것을 실행하라! ==> 없으면 걍 흰 창... 아무것도 안그려짐
}

void reshape(int w, int h) {
	glLoadIdentity();
	glViewport(0, 0, w, h); // 윈도우 크기(w, h)까지 그림을 그리는 영역(viewport)으로 지정
	gluOrtho2D(0.0, 1000, 0.0, 1000); // x축, y축 창의 맨 끝 범위가 0~100이라는 것을 알려줌 => Q. 1000으로 바꾸면? -> 전체 범위가 1000인데 네모 크기는 30~50이므로 작게 출력
}

void display_rect(void) {
	glClear(GL_COLOR_BUFFER_BIT); // 화면 지우기
	glColor3f(1.0, 0.0, 0.0); // RGB -> 따라서 빨간색. 1이 가장 밝음
	glRectf(30.3, 30.0, 50.0, 50.0); // 창 좌측하단이 원점, 따라서 좌측하단 꼭짓점이 (30, 30), 우측상단 꼭짓점이 (50, 50)
	glutSwapBuffers(); // 그래픽카드 메모리에 그린 것을 화면에 바꿔치기 -> 요즘은 그려지는 과정은 안보여야...!
}

/*
// 1. 중앙쪽 빨간 네모
void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("example");
	glutReshapeFunc(reshape); // glutReshapeFunc : 창을 마우스로 크기조절하다가 놨을 때 불리는 함수
	glutDisplayFunc(display_rect); // glutDisplayFunc : 다른 창에 가려졌다가 다시 보여졌을 때 불리는 함수
	glutMainLoop();
}


// 2. 주전자
void main(int argc, char **argv) {
	glutCreateWindow("teapot");
	glClearColor(0.0, 0.0, 0.0, 1.0); // glClearColor : 배경색 => 검정색, 마지막 1 => 완전 불투명!
	glutDisplayFunc(myDisplay_teapot);
	glutMainLoop();
}


// 3. 점들로 이루어진 원
void main(int argc, char **argv) {
	glutCreateWindow("circle");
	//glClearColor(0.0, 0.0, 0.0, 1.0); => default : black
	glutDisplayFunc(myDisplay_circle);
	glutMainLoop();
}


// 4. 선
void main(int argc, char **argv) {
	glutCreateWindow("line");
	glutDisplayFunc(myDisplay_line);
	glutMainLoop();
}
*/

// 5. 삼각형 내부 칠하기
void main(int argc, char **argv) {
	glutCreateWindow("tri_shadeModel");
	glutDisplayFunc(myDisplay_shadeModel);
	glutMainLoop();
}