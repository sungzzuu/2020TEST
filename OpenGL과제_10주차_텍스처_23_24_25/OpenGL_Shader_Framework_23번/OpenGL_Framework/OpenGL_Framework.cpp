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

	//////////////////////////////////////////////

	glutDisplayFunc(drawScene);
	glutReshapeFunc(ReShape);

	// 여기서 이벤트 처리함수 추가
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, Timer, 1);

	glutMainLoop(); // 이벤트 처리 시작 중요!!!!!!! 종료하라는 명령어 들어올때까지 실행한다
	//glutLeaveMainLoop(); // 이벤트 프로세싱을 종료(프로그램 종료)

}

GLvoid drawScene(GLvoid)
{
	glClearColor(1.0, 1.0, 1.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/////// 셰이더
	Shader_Func.Update();
	Shader_Func.Call_Vao();
	glutPostRedisplay();

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
	/*case 'z':
		Shader_Func.Move_Camera(glm::vec3(0.f, 0.f, 1.f));
		break;
	case 'Z':
		Shader_Func.Move_Camera(glm::vec3(0.f, 0.f, -1.f));
		break;
	case 'x':
		Shader_Func.Move_Camera(glm::vec3(1.f, 0.f, 0.f));
		break;
	case 'X':
		Shader_Func.Move_Camera(glm::vec3(-1.f, 0.f, 0.f));
		break;
	case 'y':
		Shader_Func.Revolution_Camera(glm::vec3(0.f, 1.f, 0.f));
		break;
	case 'Y':
		Shader_Func.Move_Camera(glm::vec3(0.f, -1.f, 0.f));
		break;
	case 'r':
		Shader_Func.Rotate_Camera(1.f);
		break;
	case 'R':
		Shader_Func.Rotate_Camera(-1.f);
		break;*/
	case 'x':
		if (bTimerAlived[0])
			bTimerAlived[0] = false;
		else
			bTimerAlived[0] = true;
		glutTimerFunc(10, Timer, 0);
		break;
	case 'y':
		if (bTimerAlived[1])
			bTimerAlived[1] = false;
		else
			bTimerAlived[1] = true;
		glutTimerFunc(10, Timer, 1);
		break;
	case 'p':
		Shader_Func.Change_Shape();
		break;
	default:
		break;
	}
}

GLvoid Timer(int value)
{
	if (value == 0 && bTimerAlived[0]) {
		Shader_Func.Rotate(glm::vec3(1.f, 0.f, 0.f));
		Shader_Func.Call_Vao();
		glutTimerFunc(10, Timer, 0);

	}
	if (value == 1 && bTimerAlived[1]) {
		Shader_Func.Rotate(glm::vec3(0.f, 1.f, 0.f));
		Shader_Func.Call_Vao();
		glutTimerFunc(10, Timer, 1);

	}
}
