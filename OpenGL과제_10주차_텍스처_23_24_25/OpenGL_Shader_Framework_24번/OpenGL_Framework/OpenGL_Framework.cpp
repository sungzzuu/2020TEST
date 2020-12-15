#include "pch.h"
#include "CShader_Func.h"
// 전역함수
GLvoid drawScene(GLvoid);
GLvoid ReShape(int w, int h);
// Func함수
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Timer(int value);

// 셰이더 클래스 변수
CShader_Func Shader_Func;

GLboolean bTimerAlived[3]{};

int main(int argc, char** argv)
{
	srand(unsigned int(time(NULL)));

	glutInit(&argc, argv);		// GLUT openGL 환경 초기화
	// 더블 모드로 해야함
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 컬러모델, 윈도우 버퍼 등 초기의 출력 모드 결정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정 100, 100 떨어진 위치에 생성
	glutInitWindowSize(WINCX, WINCY); // 윈도우의 크기 지정
	int winID = glutCreateWindow("OpenGL기초"); // 윈도우 생성 윈도우의 아이디 리턴



	//////////////////////////////////////////////////////////////////////////////////////////////


	glewExperimental = GL_TRUE;		// GLEW 초기화

	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";
	////////////셰이더 관련////////////////////////
	Shader_Func.Initialize_Shader();
	Shader_Func.Update();

	//////////////////////////////////////////////

	glutDisplayFunc(drawScene);
	glutReshapeFunc(ReShape);

	// 여기서 이벤트 처리함수 추가
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, Timer,1);

	glutMainLoop(); // 이벤트 처리 시작 중요!!!!!!! 종료하라는 명령어 들어올때까지 실행한다
	//glutLeaveMainLoop(); // 이벤트 프로세싱을 종료(프로그램 종료)


}

GLvoid drawScene(GLvoid)
{
	glClearColor(0.0, 0.0, 0.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/////// 셰이더
	Shader_Func.Call_Vao();


	glutSwapBuffers();
}

GLvoid ReShape(int w, int h)
{
	glViewport(0, 0, w, h);
}
GLvoid Mouse(int button, int state, int x, int y)
{
	return GLvoid();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	//case 'z':
	//	Shader_Func.Move_Camera(glm::vec3(0.f, 0.f, 1.f));
	//	break;
	//case 'Z':
	//	Shader_Func.Move_Camera(glm::vec3(0.f, 0.f, -1.f));
	//	break;
	//case 'x':
	//	Shader_Func.Move_Camera(glm::vec3(1.f, 0.f, 0.f));
	//	break;
	//case 'X':
	//	Shader_Func.Move_Camera(glm::vec3(-1.f, 0.f, 0.f));
	//	break;
	//case 'y':
	//	Shader_Func.Move_Camera(glm::vec3(0.f, 1.f, 0.f));
	//	break;
	//case 'Y':
	//	Shader_Func.Move_Camera(glm::vec3(0.f, -1.f, 0.f));
	//	break;
	//case 'r':
	//	if (bTimerAlived[0])
	//		bTimerAlived[0] = false;
	//	else
	//		bTimerAlived[0] = true;
	//	glutTimerFunc(10, Timer, 0);
	//	break;
	//case 'c':
	//	Shader_Func.Change_LightColor(glm::vec3(1.f, 0.f, 0.f));
	//	break;
	//case 'C':
	//	Shader_Func.Change_LightColor(glm::vec3(1.f, 1.f, 1.f));
	//	break;
	//case 'd':
	//	Shader_Func.Change_LightColor(glm::vec3(0.8f, 0.3f, 0.5f));
	//	break;
	//case 'D':
	//	Shader_Func.Change_LightColor(glm::vec3(0.8f, 0.7f, 0.2f));
	//	break;
	case 's':
		if (bTimerAlived[2])
			bTimerAlived[2] = false;
		else
			bTimerAlived[2] = true;
		glutTimerFunc(10, Timer, 2);
		break;
	case 'y':
		if (bTimerAlived[0])
			bTimerAlived[0] = false;
		else
			bTimerAlived[0] = true;
		glutTimerFunc(10, Timer, 1);
		break;
	default:
		break;
	}
	Shader_Func.Update();
	glutPostRedisplay();
}

GLvoid Timer(int value)
{

	if (1 == value && bTimerAlived[0]) {
		//Shader_Func.Recolution_Planets();
		Shader_Func.Rotate_All_Objects();
		glutTimerFunc(10, Timer, 1);
		Shader_Func.Update();
		glutPostRedisplay();
	}
	/*if (0 == value && bTimerAlived[0]) {
		Shader_Func.Move_LightPos(glm::vec3(0.f, -1.f, 0.f));
		glutTimerFunc(10, Timer, 0);
		Shader_Func.Update();
		glutPostRedisplay();
	}*/
	if (2 == value && bTimerAlived[2]) {
		Shader_Func.snow_animation();
		glutTimerFunc(10, Timer, 2);
		Shader_Func.Update();
		glutPostRedisplay();
	}

}
