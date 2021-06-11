#include <iostream>
using namespace std;

#include<glut.h>
#include<gl.h>
#include<glu.h>
#include <windows.h>
#include <stdio.h>

static int Day = 0, Time = 0, Day2 = 0, Day3 = 0; // �������� : ���������� �޸� ���α׷��� ���۵Ǹ� �ʱ�ȭ�� �� 1ȸ�� ����, ���α׷� ���� ������ �޸� �Ҹ� x, �ٸ� ���Ͽ��� ������ �� �� ����

GLUquadric *sun;
GLuint sunTexture;

int LoadBitmap(char *filename)
{
	FILE * file;
	char temp;
	long i;

	BITMAPINFOHEADER infoheader;
	unsigned char *infoheader_data;

	GLuint num_texture;

	if ((file = fopen(filename, "rb")) == NULL) return (-1); // Open the file for reading

	fseek(file, 18, SEEK_CUR);  /* start reading width & height */
	fread(&infoheader.biWidth, sizeof(int), 1, file);

	fread(&infoheader.biHeight, sizeof(int), 1, file);

	fread(&infoheader.biPlanes, sizeof(short int), 1, file);
	if (infoheader.biPlanes != 1) {
		printf("Planes from %s is not 1: %u", filename, infoheader.biPlanes);
			return 0;
	}

	// read the bpp
	fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
	if (infoheader.biBitCount != 24) {
		printf("Bpp from %s is not 24: %d", filename, infoheader.biBitCount);
			return 0;
	}

	fseek(file, 24, SEEK_CUR);

	// read the data
	if (infoheader.biWidth<0) {
		infoheader.biWidth = -infoheader.biWidth;
	}
	if (infoheader.biHeight<0) {
		infoheader.biHeight = -infoheader.biHeight;
	}
	infoheader_data = (unsigned char *)malloc(infoheader.biWidth * infoheader.biHeight * 3);
	if (infoheader_data == NULL) {
		printf("Error allocating memory for color-corrected image data");
			return 0;
	}

	if ((i = fread(infoheader_data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
		printf("Error reading image data from %s.", filename);
			return 0;
	}

	for (i = 0; i<(infoheader.biWidth * infoheader.biHeight * 3); i += 3) { // reverse all of the colors. (bgr -> rgb)
		temp = infoheader_data[i];
		infoheader_data[i] = infoheader_data[i + 2];
		infoheader_data[i + 2] = temp;
	}


	fclose(file); // Closes the file stream

	glGenTextures(1, &num_texture);
	glBindTexture(GL_TEXTURE_2D, num_texture); // Bind the ID texture specified by the 2nd parameter

											   // The next commands sets the texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Finally we define the 2d texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, infoheader_data);

	// And create 2d mipmaps for the minifying function
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, infoheader_data);

	free(infoheader_data); // Free the memory we used to load the texture

	return (num_texture); // Returns the current texture OpenGL ID
}


void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 1.0, 0.0);  // ��ü ���� ����� (ī�޶� ��ġ �̵�)

	glEnable(GL_LIGHTING); // ������ ����� ���̴� (���� Ȱ��ȭ)
	glEnable(GL_LIGHT0); // ���� �� 0�� ������ ����� ���̴� (���� on)

	//glColor3f(1.0, 0.3, 0.3); // �¾��� ��
	//glutSolidSphere(0.13, 20, 16); // �¾��� �׷��� // glutSolidSphere() : ���� ��, gluSphere : �� ���, glutWireSphere : ���̾� �� // (������, ���� �� ��, ���� �� ��)

	// ���� ����
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1 };  // (x, y, z, w) : w = 0�̸� �Ÿ� ������ ���� �¾籤(���Ɽ), 1�̸� ����������� ���� ��ġ��
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // ���� ��ġ ����
	GLfloat light_diff[] = { 1.0, 1.0, 1.0, 1.0 }; // (R, G, B, A) 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff); // �л걤 ���� ����
	GLfloat light_amb[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb); // �ֺ��� ���� ����
	
	glEnable(GL_COLOR_MATERIAL); // �� �ڵ� �־���� light enable �Ŀ��� vertex color ��� ���� (light enable�� �ϸ� ���� ������ �ƴ� ���� material �Ӽ����� �� ����) => �� ������ ���� �������� ������ �ϰڴٴ� �ڵ�
	
	glPushMatrix(); // �¾������ ��ǥ�踦 ���ÿ� Ǫ���Ͽ� ����
	glRotatef((GLfloat)Day, 0.0, 1.0, 0.0); // ȸ���� (0, 1, 0) : Y�� �������� Day ����ŭ ȸ��
	glTranslatef(0.45, 0.0, 0.0); // �¾����κ��� 0.45��ŭ ������ �Ÿ�����ġ
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0); 
	glColor3f(0.5, 0.6, 0.7);
	glutSolidSphere(0.06, 100, 100); // ������ �׷���

	glPushMatrix(); // ���������� ��ǥ�踦 ���ÿ� Ǫ���Ͽ� ����
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glTranslatef(0.12, 0.0, 0.0); // �����κ��� 0.12 ������ �Ÿ��� ��ġ
	glColor3f(0.9, 0.8, 0.2); // ���� �� ����
	glutSolidSphere(0.015, 100, 100); // ���� �׷���
	glPopMatrix();
	glPopMatrix();

	glLoadIdentity(); // ���� ��ǥ ���� �ʱ�ȭ
	gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 1.0, 1.03, 0.0); // �¾�� �༺���� ���� �����ϰ� �������ִٰ� �Ǿ� �ֱ⿡ �ణ�� ���� ���̸� ��
	glRotatef((GLfloat)Day2, 0.0, 1.0, 0.0);
	glTranslatef(0.65, 0.0, 0.0); // �¾����κ��� 0.65 ������ �Ÿ��� ��ġ
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glColor3f(0.9, 0.8, 0.1); // �ݼ��� �� ����
	glutSolidSphere(0.05, 100, 100); // �ݼ��� �׷���
	
	glLoadIdentity();
	gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 1.0, 1.025, 0.0);
	glRotatef((GLfloat)Day3, 0.0, 1.0, 0.0);
	glTranslatef(0.95, 0.0, 0.0); // �¾����κ��� 0.95 ������ �Ÿ��� ��ġ
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 1.0); // ������ �� ����
	glutSolidSphere(0.026, 100, 100); // ������ �׷���


	// sun
	glLoadIdentity();
	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sunTexture);

	glColor3f(1.0, 1.0, 1.0); // texture�� ������ ǥ���Ϸ��� color(1, 1, 1) ����� ��
	gluSphere(sun, 0.13, 20, 20);
	glDisable(GL_TEXTURE_2D); // �̰� ����� �ٸ� �༺�鿡�� ���� �ȹ�ħ


	glFlush();
	glutSwapBuffers();
}

void MyKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'd':
		Day = (Day + 1) % 360;
		glutPostRedisplay(); // �����츦 �ٽ� �׸����� ��û(���ΰ�ħ) => ���� ���÷��� �ݹ��Լ�(MyDisplay) ȣ��
		Time = (Time + 10) % 360;
		glutPostRedisplay();
		Day2 = (Day2 + 2) % 360;  // �ݼ��� �����ֱ�� 227�� ������ 0.6 ȸ���� 1�� ������ ȸ���� ������
								  // �Ҽ��� ������ ǥ���� �����Ͽ� ���� 1�� ȸ���� 2��ȸ���� ��Ŵ
		glutPostRedisplay();
		Day3 = (Day3 + 4) % 360;  // ������ �����ֱ�� 87�� ������ 1�� ȸ���� 4�� ȸ��
		glutPostRedisplay();

		break;
	default:
		break;
	}
}

void init(void)
{
	glEnable(GL_DEPTH_TEST); // ***���� �׽�Ʈ Ȱ��ȭ***

	sun = gluNewQuadric();
	gluQuadricTexture(sun, GL_TRUE);
	sunTexture = LoadBitmap("sun.bmp");
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0); // ȭ�� �»�� �������� â ��ġ��Ű��
	glutCreateWindow("Solar System");
	glClearColor(0.0, 0.0, 0.0, 1.0); // ��� : ������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	init();
	glutDisplayFunc(MyDisplay); // ȭ�鿡 �׸� �� ȣ��Ǵ� �ݹ��Լ�
	glutKeyboardFunc(MyKeyboard); // Ű���尡 ���� ������ ȣ��Ǵ� �ݹ��Լ�
	glutMainLoop();
	return 0;
}