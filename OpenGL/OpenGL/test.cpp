#include <windows.h>
#include <glut.h>
#include <math.h>
#define Pi 3.14



void myDisplay_shadeModel() {
	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(GL_SMOOTH); // �׸� �ٰ����� ���δ� �׶��̼����� ĥ�� ��
	//glShadeModel(GL_FLAT); // �ܻ� (���� ���� ���� ���� => ��, ���� ������ �ڵ��� ������ B!!)

	glBegin(GL_TRIANGLES); // �ﰢ�� �׸��� ����
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

	glBegin(GL_LINES); // ���׸��� -> �� �� ����

	glLineWidth(5.0); // �� �β�
	glColor3f(1.0, 0.0, 0.0); // ����
	glVertex3f(1.0, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0); // �ʷ�
	glVertex3f(-1.0, 0.0, 0.0);
	 
	glColor3f(0.0, 0.0, 1.0); // �Ķ�
	glVertex3f(0.5, 0.5, 0.0); // ***x���� 0.5�� �ٲٸ� â ���� ũ���� 0.5�� ��ġ�� �� ����***
	glColor3f(0.0, 1.0, 1.0); // �ϴû� (G+B)
	glVertex3f(-1.0, 0.5, 0.0);

	glEnd();

	glFlush(); 
}


void myDisplay_circle() {
	GLfloat size[2]; // ũ�Ⱑ 2¥���� size �迭 ����. �ڷ����� GLfloat type
	GLfloat angle; // �Ʒ� for������ ���� ���� angle ���� ����
	glClear(GL_COLOR_BUFFER_BIT);
	// �������
	glColor3f(1.0, 1.0, 1.0); // ���
	glGetFloatv(GL_POINT_SIZE_RANGE, size); // HW�� ���������� ���� ũ�⸦ size��� �迭�� ����
	//glPointSize(size[1]); // size[0] : �� �� ���� ���� ũ���� ������ �׸� ���� ũ�� ���� (size[1] : ���� ū ũ�� ��)
	glPointSize((size[1] - size[0]) / 2.0 + size[0]); // ��� ũ���� ��~
	glBegin(GL_POINTS); // ������ ��� ���� ����~
	
	// 0 radian���� 2���� radian ==> ��, 0~360������, ����/30 = 180/30 = 6�� �������� �� ��� => ��Ȯ���� �� 61�� ���� (������ ���ļ�)
	for (angle = 0.0; angle <= 2.0 * Pi; angle += Pi / 30.0) {
		glVertex3f(0.5 * cos(angle), 0.5 * sin(angle), 0.0);
	}
	glEnd(); // ������ �׳� �� â. �ƹ��͵� �ȱ׷���
	// ������� �߿��� �κ�
	glFlush(); 
}

void myDisplay_teapot() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 0.0); //����� Ƽ��
	glutSolidTeapot(0.5); // ������ ����
	glColor3f(1.0, 0.0, 0.0); // ������
	glutWireTeapot(0.5); // ���̾� Ƽ��!

	glFlush(); // �׷��� ���α׷����� ���� �����϶�! ==> ������ �� �� â... �ƹ��͵� �ȱ׷���
}

void reshape(int w, int h) {
	glLoadIdentity();
	glViewport(0, 0, w, h); // ������ ũ��(w, h)���� �׸��� �׸��� ����(viewport)���� ����
	gluOrtho2D(0.0, 1000, 0.0, 1000); // x��, y�� â�� �� �� ������ 0~100�̶�� ���� �˷��� => Q. 1000���� �ٲٸ�? -> ��ü ������ 1000�ε� �׸� ũ��� 30~50�̹Ƿ� �۰� ���
}

void display_rect(void) {
	glClear(GL_COLOR_BUFFER_BIT); // ȭ�� �����
	glColor3f(1.0, 0.0, 0.0); // RGB -> ���� ������. 1�� ���� ����
	glRectf(30.3, 30.0, 50.0, 50.0); // â �����ϴ��� ����, ���� �����ϴ� �������� (30, 30), ������� �������� (50, 50)
	glutSwapBuffers(); // �׷���ī�� �޸𸮿� �׸� ���� ȭ�鿡 �ٲ�ġ�� -> ������ �׷����� ������ �Ⱥ�����...!
}

/*
// 1. �߾��� ���� �׸�
void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("example");
	glutReshapeFunc(reshape); // glutReshapeFunc : â�� ���콺�� ũ�������ϴٰ� ���� �� �Ҹ��� �Լ�
	glutDisplayFunc(display_rect); // glutDisplayFunc : �ٸ� â�� �������ٰ� �ٽ� �������� �� �Ҹ��� �Լ�
	glutMainLoop();
}


// 2. ������
void main(int argc, char **argv) {
	glutCreateWindow("teapot");
	glClearColor(0.0, 0.0, 0.0, 1.0); // glClearColor : ���� => ������, ������ 1 => ���� ������!
	glutDisplayFunc(myDisplay_teapot);
	glutMainLoop();
}


// 3. ����� �̷���� ��
void main(int argc, char **argv) {
	glutCreateWindow("circle");
	//glClearColor(0.0, 0.0, 0.0, 1.0); => default : black
	glutDisplayFunc(myDisplay_circle);
	glutMainLoop();
}


// 4. ��
void main(int argc, char **argv) {
	glutCreateWindow("line");
	glutDisplayFunc(myDisplay_line);
	glutMainLoop();
}
*/

// 5. �ﰢ�� ���� ĥ�ϱ�
void main(int argc, char **argv) {
	glutCreateWindow("tri_shadeModel");
	glutDisplayFunc(myDisplay_shadeModel);
	glutMainLoop();
}