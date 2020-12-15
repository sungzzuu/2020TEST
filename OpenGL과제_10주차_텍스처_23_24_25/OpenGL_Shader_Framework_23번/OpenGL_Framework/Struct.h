#pragma once


typedef struct modelTransform
{
	glm::vec3 Scale{1.f, 1.f, 1.f};
	glm::vec3 Rotate{ 30.f, -30.f, 0.f };
	glm::vec3 Translate{ 0.f, 0.f, 0.f };
	glm::vec3 Revolution{ 0.f, 0.f, 0.f };
	glm::vec3 pivot{ 0.f,0.f,0.f };
	glm::vec3 origin_Translate{ 0.f, 0.f, 0.f };
	modelTransform* Parent = nullptr;

	glm::mat4 matWorld;
}MODEL_TRANSFORM;

typedef struct Triangle
{
	glm::vec3 pos[3];
	glm::vec3 color[3];

}TRIANGLE;

typedef struct Line
{
	glm::vec3 pos[2]{ {-0.5f,0.f,0.f},{0.5f,0.f,0.f} };

	glm::vec3 color[2]{ {-0.5f,0.5f,0.5f},{-0.5f,0.5f,0.5f}};

}LINE;

typedef struct Circle
{
	glm::vec3 pos[360]{};
	glm::vec3 color[360]{};

	// ũ�� ���� �̵� ���� �θ�
	MODEL_TRANSFORM model_transform;
}CIRCLE;

typedef struct Camera
{
	Camera()
	{
		cameraPos = { 0.f, 0.f, 5.f };
		cameraDirection = glm::vec3(0.f, 0.f, 0.f);
		cameraUp = glm::vec3(0.f, 1.0f, 0.f);
	}
	glm::vec3 cameraPos;
	glm::vec3 cameraDirection;
	glm::vec3 cameraUp;
	MODEL_TRANSFORM model_transform;

}CAMERA_DESC;


typedef struct Object 
{
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	vector<glm::vec3> color;

	MODEL_TRANSFORM model_transform;

}OBJECT;