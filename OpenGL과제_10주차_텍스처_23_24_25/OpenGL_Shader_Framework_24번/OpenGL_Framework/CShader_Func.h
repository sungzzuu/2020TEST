#pragma once
namespace PLAYER
{
	enum BODY { UNDER, CENTER, LEFT_ARM, RIGHT_ARM, BODY_END };

}
class CShader_Func
{
public:
	CShader_Func();
	~CShader_Func();

public:
	
	// 1. drawScene함수에서 그리기 for문으로
	// 2. make_vertexShader()
	// 3. make_fragmentShader()
	// 4. InitBuffer()
	// 5. InitShader()

	// 초기화
	void Initialize_Shader();
	void make_vertexShader();
	void make_fragmentShader();
	void Init_Player();

	// 업데이트
	void Update();
	void Update_Buffer(int _vindex, vector<glm::vec3> _vertices, vector<glm::vec3> _normals, vector<glm::vec3> _color);
	void Update_Buffer_column(int _vindex, vector<glm::vec3> _vertices, vector<glm::vec3> _normals, vector<glm::vec4> _color);
	void Update_Shader();
	void Call_Vao();


	// 모델변환 적용
	void Set_ModelTransform(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation);
	void Set_ModelTransform_Column(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation);

	// Obj불러오기
	bool loadObj(const char* path,
		vector<glm::vec3>& out_vertices,
		vector<glm::vec2>& out_uvs,
		vector<glm::vec3>& out_normals,
		vector<glm::vec3>& out_color,
		glm::vec3 _color);

	bool loadObj_RGBA(const char* path,
		vector<glm::vec3>& out_vertices,
		vector<glm::vec2>& out_uvs,
		vector<glm::vec3>& out_normals,
		vector<glm::vec4>& out_color_rgba,
		glm::vec4 _color);

	// 조명 위치 설정
	void Move_LightPos(glm::vec3 _MoveSize);
	void Change_LightColor(glm::vec3 _color);

	// 카메라
	void Move_Camera(glm::vec3 _moveSize);

	// 행성 공전
	void Recolution_Planets();

	// 눈 떨어지고 올라가고 반복
	void snow_animation();

	// 쌓이는 눈 Init
	void Init_build_snow();

	// 24번 함수
	void Rotate_All_Objects();
private:
	GLchar* vertexsource, * fragmentsource; // 소스코드 저장 변수
	GLuint vertexshader, fragmentshader; // 세이더 객체
	GLuint s_program;
	//TRIANGLE triShape[TRINUM];
	//CUBE cubeShape[CUBENUM];
	//LINE lineShape[LINENUM];
	GLuint vao[100], vbo[100][2];
	GLuint normalbuffer;
	GLenum ePolygonMode = GL_FILL;
	CAMERA_DESC m_Camera_Desc;
	GLfloat m_Camera_Rotate_Angle = 0.f;
	glm::vec3 Camera_Pos{ 0.f, 3.f, 15.f };


	// obj 불러오기
	// 여기에 넣는다.
	// 각 객체는 좌표가 있다.
	OBJECT sphere[3];
	OBJECT bottom;

	GLuint object_num = 0;

	OBJECT light;

	glm::vec3 Light_Pos{-4.f, 0.f, -5.f};
	glm::vec3 Light_Rotate{ 0.f, 0.f, 0.f };
	glm::vec3 Light_Color{ 1.f, 1.f, 1.f };

	OBJECT pyramid;


	OBJECT snow[30];
	list<OBJECT*> snow_build;
	GLuint buildCnt = 0;

	// 24번 문제
	OBJECT columns[6];
};

