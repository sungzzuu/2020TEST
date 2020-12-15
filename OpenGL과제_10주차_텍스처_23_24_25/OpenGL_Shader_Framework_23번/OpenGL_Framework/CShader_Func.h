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
	
	// 1. drawScene�Լ����� �׸��� for������
	// 2. make_vertexShader()
	// 3. make_fragmentShader()
	// 4. InitBuffer()
	// 5. InitShader()

	// �ʱ�ȭ
	void Initialize_Shader();
	void make_vertexShader();
	void make_fragmentShader();
	void Init_Objects();
	void Init_Texture();

	// ������Ʈ
	void Update();
	void Update_Buffer(int _vindex, vector<glm::vec3> _vertices, vector<glm::vec2> _uv);
	void Update_Shader();
	void Call_Vao();

	// ī�޶� �̵� �Լ�
	void Move_Camera(glm::vec3 _moveSize);
	void Revolution_Camera(glm::vec3 _moveSize);
	void Rotate_Camera(GLfloat _dir);
	// �𵨺�ȯ ����
	void Set_ModelTransform(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation);
	void Set_ModelTransform_Parent(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation);
	void Set_ArmModelTransform(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation);

	// Obj�ҷ�����
	bool loadObj(const char* path,
		vector<glm::vec3>& out_vertices,
		vector<glm::vec2>& out_uvs,
		vector<glm::vec3>& out_normals,
		vector<glm::vec3>& out_color,
		glm::vec3 _color);
	
	// ��Ʈ�� �ҷ�����
	GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info);

	// ��Ʈ�� �ҷ����� ����Ʈ
	GLuint loadBMP_custom(const char* imagepath);

	// 23�� 25�� �Լ�
	void Rotate(glm::vec3 _RotSize);
	void Change_Shape();
private:
	GLchar* vertexsource, * fragmentsource; // �ҽ��ڵ� ���� ����
	GLuint vertexshader, fragmentshader; // ���̴� ��ü
	GLuint s_program;
	//TRIANGLE triShape[TRINUM];
	//CUBE cubeShape[CUBENUM];
	//LINE lineShape[LINENUM];
	GLuint vao[6], vbo[6], uv_vbo[6], texture[6];
	GLenum ePolygonMode = GL_FILL;
	CAMERA_DESC m_Camera_Desc;
	GLfloat m_Camera_Rotate_Angle = 0.f;
	glm::vec3 Camera_Pos{ 0.f, 0.f, 5.f };


	// obj �ҷ�����
	// ���⿡ �ִ´�.
	// �� ��ü�� ��ǥ�� �ִ�.

	GLboolean bBegin = true;

	OBJECT cube;
	GLuint VertexArrayID;
	GLuint polygonNum = 6;
};

