#pragma once
namespace PLAYER
{
	enum BODY { UNDER, CENTER, LEFT_ARM, RIGHT_ARM, BODY_END };
	enum DIR {FRONT, BACK, LEFT, RIGHT};
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
	void Init_Objects();
	void Init_Texture();

	// 업데이트
	void Update();
	void Update_Buffer(int _vindex, vector<glm::vec3> _vertices, vector<glm::vec2> _uv);
	void Update_Shader();
	void Call_Vao();

	// 카메라 이동 함수
	void Move_Camera(glm::vec3 _moveSize);
	void Revolution_Camera(glm::vec3 _moveSize);
	void Rotate_Camera(GLfloat _dir);
	// 모델변환 적용
	void Set_ModelTransform(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation);
	void Set_ModelTransform_Parent(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation);
	void Set_ArmModelTransform(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation);

	// Obj불러오기
	bool loadObj(const char* path,
		vector<glm::vec3>& out_vertices,
		vector<glm::vec2>& out_uvs,
		vector<glm::vec3>& out_normals,
		vector<glm::vec3>& out_color,
		glm::vec3 _color);
	
	// 비트맵 불러오기
	GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info);

	// 비트맵 불러오기 사이트
	GLuint loadBMP_custom(const char* imagepath);

	// 23번 25번 함수
	void Rotate(glm::vec3 _RotSize);
	void Change_Shape();
private:
	GLchar* vertexsource, * fragmentsource; // 소스코드 저장 변수
	GLuint vertexshader, fragmentshader; // 세이더 객체
	GLuint s_program;
	//TRIANGLE triShape[TRINUM];
	//CUBE cubeShape[CUBENUM];
	//LINE lineShape[LINENUM];
	GLuint vao[6], vbo[6], uv_vbo[6], texture[6];
	GLenum ePolygonMode = GL_FILL;
	CAMERA_DESC m_Camera_Desc;
	GLfloat m_Camera_Rotate_Angle = 0.f;
	glm::vec3 Camera_Pos{ 0.f, 0.f, 5.f };


	// obj 불러오기
	// 여기에 넣는다.
	// 각 객체는 좌표가 있다.

	GLboolean bBegin = true;

	OBJECT cube;
	GLuint VertexArrayID;
	GLuint polygonNum = 6;
};

