#include <iostream>
using namespace std;

#include<glut.h>
#include<gl.h>
#include<glu.h>
#include <windows.h>
#include <stdio.h>

static int Day = 0, Time = 0, Day2 = 0, Day3 = 0; // 정적변수 : 전역변수와 달리 프로그램이 시작되면 초기화는 딱 1회만 수행, 프로그램 종료 전까지 메모리 소멸 x, 다른 파일에서 가져다 쓸 수 없음

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
	gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 1.0, 0.0);  // 전체 모델을 기울임 (카메라 위치 이동)

	glEnable(GL_LIGHTING); // 조명을 사용할 것이다 (조명 활성화)
	glEnable(GL_LIGHT0); // 조명 중 0번 조명을 사용할 것이다 (조명 on)

	//glColor3f(1.0, 0.3, 0.3); // 태양의 색
	//glutSolidSphere(0.13, 20, 16); // 태양을 그려냄 // glutSolidSphere() : 꽉찬 구, gluSphere : 구 곡면, glutWireSphere : 와이어 구 // (반지름, 세로 면 수, 가로 면 수)

	// 조명 설정
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1 };  // (x, y, z, w) : w = 0이면 거리 제한이 없는 태양광(지향광), 1이면 전구램프등과 같은 위치광
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // 광원 위치 설정
	GLfloat light_diff[] = { 1.0, 1.0, 1.0, 1.0 }; // (R, G, B, A) 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff); // 분산광 성분 설정
	GLfloat light_amb[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb); // 주변광 성분 설정
	
	glEnable(GL_COLOR_MATERIAL); // 이 코드 넣어줘야 light enable 후에도 vertex color 사용 가능 (light enable만 하면 현재 색상이 아닌 현재 material 속성으로 색 결정) => 즉 재질의 색을 폴리곤의 색으로 하겠다는 코드
	
	glPushMatrix(); // 태양기준의 좌표계를 스택에 푸시하여 저장
	glRotatef((GLfloat)Day, 0.0, 1.0, 0.0); // 회전축 (0, 1, 0) : Y축 기준으로 Day 각만큼 회전
	glTranslatef(0.45, 0.0, 0.0); // 태양으로부터 0.45만큼 떨어진 거리에위치
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0); 
	glColor3f(0.5, 0.6, 0.7);
	glutSolidSphere(0.06, 100, 100); // 지구를 그려냄

	glPushMatrix(); // 지구기준의 좌표계를 스택에 푸시하여 저장
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glTranslatef(0.12, 0.0, 0.0); // 지구로부터 0.12 떨어진 거리에 위치
	glColor3f(0.9, 0.8, 0.2); // 달의 색 지정
	glutSolidSphere(0.015, 100, 100); // 달을 그려냄
	glPopMatrix();
	glPopMatrix();

	glLoadIdentity(); // 전역 좌표 기준 초기화
	gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 1.0, 1.03, 0.0); // 태양계 행성들이 거의 유사하게 기울어져있다고만 되어 있기에 약간의 기울기 차이만 줌
	glRotatef((GLfloat)Day2, 0.0, 1.0, 0.0);
	glTranslatef(0.65, 0.0, 0.0); // 태양으로부터 0.65 떨어진 거리에 위치
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glColor3f(0.9, 0.8, 0.1); // 금성의 색 지정
	glutSolidSphere(0.05, 100, 100); // 금성을 그려냄
	
	glLoadIdentity();
	gluLookAt(0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 1.0, 1.025, 0.0);
	glRotatef((GLfloat)Day3, 0.0, 1.0, 0.0);
	glTranslatef(0.95, 0.0, 0.0); // 태양으로부터 0.95 떨어진 거리에 위치
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 1.0); // 수성의 색 지정
	glutSolidSphere(0.026, 100, 100); // 수성을 그려냄


	// sun
	glLoadIdentity();
	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sunTexture);

	glColor3f(1.0, 1.0, 1.0); // texture만 오롯이 표현하려면 color(1, 1, 1) 해줘야 함
	gluSphere(sun, 0.13, 20, 20);
	glDisable(GL_TEXTURE_2D); // 이거 해줘야 다른 행성들에게 영향 안미침


	glFlush();
	glutSwapBuffers();
}

void MyKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'd':
		Day = (Day + 1) % 360;
		glutPostRedisplay(); // 윈도우를 다시 그리도록 요청(새로고침) => 이후 디스플레이 콜백함수(MyDisplay) 호출
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

void init(void)
{
	glEnable(GL_DEPTH_TEST); // ***깊이 테스트 활성화***

	sun = gluNewQuadric();
	gluQuadricTexture(sun, GL_TRUE);
	sunTexture = LoadBitmap("sun.bmp");
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0); // 화면 좌상단 꼭짓점에 창 위치시키기
	glutCreateWindow("Solar System");
	glClearColor(0.0, 0.0, 0.0, 1.0); // 배경 : 검은색
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	init();
	glutDisplayFunc(MyDisplay); // 화면에 그릴 때 호출되는 콜백함수
	glutKeyboardFunc(MyKeyboard); // 키보드가 눌릴 때마다 호출되는 콜백함수
	glutMainLoop();
	return 0;
}