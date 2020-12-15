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
	
	// 1. drawScene�Լ����� �׸��� for������
	// 2. make_vertexShader()
	// 3. make_fragmentShader()
	// 4. InitBuffer()
	// 5. InitShader()

	// �ʱ�ȭ
	void Initialize_Shader();
	void make_vertexShader();
	void make_fragmentShader();
	void Init_Player();

	// ������Ʈ
	void Update();
	void Update_Buffer(int _vindex, vector<glm::vec3> _vertices, vector<glm::vec3> _normals, vector<glm::vec3> _color);
	void Update_Buffer_column(int _vindex, vector<glm::vec3> _vertices, vector<glm::vec3> _normals, vector<glm::vec4> _color);
	void Update_Shader();
	void Call_Vao();


	// �𵨺�ȯ ����
	void Set_ModelTransform(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation);
	void Set_ModelTransform_Column(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation);

	// Obj�ҷ�����
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

	// ���� ��ġ ����
	void Move_LightPos(glm::vec3 _MoveSize);
	void Change_LightColor(glm::vec3 _color);

	// ī�޶�
	void Move_Camera(glm::vec3 _moveSize);

	// �༺ ����
	void Recolution_Planets();

	// �� �������� �ö󰡰� �ݺ�
	void snow_animation();

	// ���̴� �� Init
	void Init_build_snow();

	// 24�� �Լ�
	void Rotate_All_Objects();
private:
	GLchar* vertexsource, * fragmentsource; // �ҽ��ڵ� ���� ����
	GLuint vertexshader, fragmentshader; // ���̴� ��ü
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


	// obj �ҷ�����
	// ���⿡ �ִ´�.
	// �� ��ü�� ��ǥ�� �ִ�.
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

	// 24�� ����
	OBJECT columns[6];
};

