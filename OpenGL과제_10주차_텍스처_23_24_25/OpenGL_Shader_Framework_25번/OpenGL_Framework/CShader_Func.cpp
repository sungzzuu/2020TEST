#include "pch.h"
#include "CShader_Func.h"

CShader_Func::CShader_Func()
{
	
}

CShader_Func::~CShader_Func()
{
}

void CShader_Func::Call_Vao()
{
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// ============================= 고정 ============================================
	glLineWidth(1.f);
	glPolygonMode(GL_FRONT_AND_BACK, ePolygonMode);
	// 사용할 VAO 불러오기
	// s_program 셰이더 사용 모델변환
	unsigned int modelLocation = glGetUniformLocation(s_program, "model");
	unsigned int viewLocation = glGetUniformLocation(s_program, "view");
	unsigned int projLocation = glGetUniformLocation(s_program, "projection");
	unsigned int tLocation = glGetUniformLocation(s_program, "outTexture");

	// ============================= 고정 ============================================
	
	
	// ============================= 뷰 변환 ============================================
	// 카메라 좌표 설정
	glm::vec3 cameraPos = Camera_Pos;
	glm::vec3 cameraDirection = m_Camera_Desc.cameraDirection;
	glm::vec3 cameraUp = m_Camera_Desc.cameraUp;
	glm::mat4 view = glm::mat4(1.f);
	glm::mat4 projection = glm::mat4(1.f);
	glm::mat4 result = glm::mat4(1.f);
	glUseProgram(s_program);

	Update_Buffer_background();
	// 배경
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_background);
	glUniform1i(tLocation, 0);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
	Set_ModelTransform(background.model_transform, modelLocation);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_background);

	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo_background);

	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	///////


	glm::mat4 view_Rotate = glm::mat4(1.f);
	glm::mat4 view_Translate = glm::mat4(1.f);
	glm::mat4 view_Revolution = glm::mat4(1.f);

	view_Rotate = glm::rotate(view_Rotate, glm::radians(m_Camera_Rotate_Angle), glm::vec3(0.0, 1.0, 0.0));
	view_Translate = glm::translate(view_Translate, m_Camera_Desc.model_transform.Translate);
	view_Revolution = glm::rotate(view_Revolution, glm::radians(m_Camera_Desc.model_transform.Revolution.y), glm::vec3(0.0, 1.0, 0.0));


	view = view_Translate * view_Rotate * glm::lookAt(cameraPos, cameraDirection, cameraUp)* view_Revolution;
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	// ============================= 뷰 변환 ============================================

	// ============================= 투영 변환 ============================================
	projection = glm::perspective(glm::radians(45.f), (float)WINCX / (float)WINCY, 1.f, 100.f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
	// ============================= 투영 변환 ============================================

	glm::mat4 Tx = glm::mat4(1.0f);
	glm::mat4 Rz = glm::mat4(1.0f);
	glm::mat4 TR = glm::mat4(1.0f);

	Tx = glm::translate(Tx, glm::vec3(0.0, 0.0, 0.0));
	Rz = glm::rotate(Rz, glm::radians(85.f), glm::vec3(1.0, 0.0, 0.0));

	TR = Tx * Rz;

	
	// 행렬 적용
		
	int vIndex = 0;
	Update_Buffer(0, cube.vertices, cube.uvs);

	Set_ModelTransform(cube.model_transform, modelLocation);
	glActiveTexture(GL_TEXTURE0);

	for (int i = 0; i < 6; ++i) {
		if (4 == polygonNum && i == 5) {
			glBindTexture(GL_TEXTURE_2D, texture[4]);
		}
		else
			glBindTexture(GL_TEXTURE_2D, texture[i]);

		glUniform1i(tLocation, 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);

		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uv_vbo[i]);

		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);



		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
	
}

void CShader_Func::Move_Camera(glm::vec3 _moveSize)
{
	Camera_Pos += _moveSize * 0.1f;
	m_Camera_Desc.cameraDirection += _moveSize * 0.1f;
}

void CShader_Func::Revolution_Camera(glm::vec3 _moveSize)
{

	m_Camera_Desc.model_transform.Revolution.y += _moveSize.y;
}

void CShader_Func::Rotate_Camera(GLfloat _dir)
{
	m_Camera_Rotate_Angle += _dir;
}


void CShader_Func::Initialize_Shader()
{


	make_vertexShader();
	make_fragmentShader();
	Init_Texture();

	Update_Shader();

	//bool res = loadObj("cube1212.obj", cube.vertices, cube.uvs, cube.normals, cube.color, glm::vec3(1.f,0.f,0.f));
	loadObj("cube1212.obj", background.vertices, background.uvs, background.normals, background.color, glm::vec3(1.f, 0.f, 0.f));
	Change_Shape();
	Init_Objects();

	int vIndex = 0;

}

void CShader_Func::make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexshader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}

void CShader_Func::make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentshader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}

void CShader_Func::Init_Objects()
{


	// 카메라
	//m_Camera_Desc.model_transform.Translate.z = -6.f;
	//m_Camera_Desc.model_transform.Translate.x = -6.f;
	background.model_transform.Rotate = { 0.f,0.f,0.f };

	background.model_transform.Translate.z += -10.f;
	background.model_transform.Scale = {1.f,1.f,1.0f};


}

void CShader_Func::Init_Texture()
{
	
	texture[0] = loadBMP_custom("A.bmp");
	texture[1] = loadBMP_custom("B.bmp");
	texture[2] = loadBMP_custom("C.bmp");
	texture[3] = loadBMP_custom("D.bmp");
	texture[4] = loadBMP_custom("E.bmp");
	texture[5] = loadBMP_custom("F.bmp");
	

	texture_background = loadBMP_custom("background.bmp");
	glGenVertexArrays(1, &vao_background);
	glGenBuffers(1, &vbo_background);
	glGenBuffers(1, &uv_vbo_background);
}

void CShader_Func::Update()
{

	int vIndex = 0;


	//Update_Buffer(vIndex++, cube.vertices, cube.uvs);

	

	Call_Vao();

}

void CShader_Func::Update_Buffer(int _vindex, vector<glm::vec3> _vertices, vector<glm::vec2> _uv)
{
	
	

	// 버텍스 6개 씩
	if (6 == polygonNum) {
		glGenVertexArrays(6, vao);
		glGenBuffers(6, vbo);
		glGenBuffers(6, uv_vbo);
		for (int i = 0; i < 6; ++i) {
			glBindVertexArray(vao[i]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
			glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), &_vertices[6 * i], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, uv_vbo[i]);
			glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), &_uv[6 * i], GL_STATIC_DRAW);
		}
	}
	if (4 == polygonNum) {
		glGenVertexArrays(4, vao);
		glGenBuffers(4, vbo);
		glGenBuffers(4, uv_vbo);
		for (int i = 0; i < 6; ++i) {
			glBindVertexArray(vao[i]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
			if (i == 4) {
				glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), &_vertices[3 * i], GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, uv_vbo[i]);
				glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), &_uv[3 * i], GL_STATIC_DRAW);
			}
			else if (i == 5) {
				glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), &_vertices[18], GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, uv_vbo[i]);
				glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), &_uv[18], GL_STATIC_DRAW);
			}
			else {
				glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &_vertices[3 * i], GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, uv_vbo[i]);
				glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), &_uv[3 * i], GL_STATIC_DRAW);
			}
			
		}
	}

	
}

void CShader_Func::Update_Buffer_background()
{
	glBindVertexArray(vao_background);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_background);
	glBufferData(GL_ARRAY_BUFFER, background.vertices.size() * sizeof(glm::vec3), &background.vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo_background);
	glBufferData(GL_ARRAY_BUFFER, background.uvs.size() * sizeof(glm::vec3), &background.uvs[0], GL_STATIC_DRAW);
}

void CShader_Func::Update_Shader()
{
	//make_vertexShader(i);
		//make_fragmentShader(i);
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	GLint result;
	GLchar errorLog[512];

	glGetProgramiv(s_program, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(s_program, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		return;
	}
	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program 사용하기
}


void CShader_Func::Set_ModelTransform(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation)
{
	glm::mat4 Scale = glm::mat4(1.f);
	
	glm::mat4 RotateX = glm::mat4(1.f);
	glm::mat4 RotateY = glm::mat4(1.f);
	glm::mat4 RotateZ = glm::mat4(1.f);

	glm::mat4 Translate = glm::mat4(1.f);

	glm::mat4 RevolutionX = glm::mat4(1.f);
	glm::mat4 RevolutionY = glm::mat4(1.f);
	glm::mat4 RevolutionZ = glm::mat4(1.f);

	glm::mat4 Parent = glm::mat4(1.f);

	glm::mat4 Result = glm::mat4(1.f);

	Scale = glm::scale(Scale, _modelTransform.Scale);

	RotateX = glm::rotate(RotateX, glm::radians(_modelTransform.Rotate.x), glm::vec3(1.0, 0.0, 0.0));
	RotateY = glm::rotate(RotateY, glm::radians(_modelTransform.Rotate.y), glm::vec3(0.0, 1.0, 0.0));
	RotateZ = glm::rotate(RotateZ, glm::radians(_modelTransform.Rotate.z), glm::vec3(0.0, 0.0, 1.0));

	Translate = glm::translate(Translate, _modelTransform.Translate);

	RevolutionX = glm::rotate(RevolutionX, glm::radians(_modelTransform.Revolution.x), glm::vec3(1.0, 0.0, 0.0));
	RevolutionY = glm::rotate(RevolutionY, glm::radians(_modelTransform.Revolution.y), glm::vec3(0.0, 1.0, 0.0));
	RevolutionZ = glm::rotate(RevolutionZ, glm::radians(_modelTransform.Revolution.z), glm::vec3(0.0, 0.0, 1.0));
	if(_modelTransform.Parent)
		Parent = glm::translate(Parent, _modelTransform.Parent->Translate);

	// 크 자 이 공 부 거꾸로
	Result = RotateX * RotateY * RotateZ * Scale;


	glUniformMatrix4fv(_modelLocation, 1, GL_FALSE, glm::value_ptr(Result));
}

void CShader_Func::Set_ModelTransform_Parent(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation)
{
	// 부모의 이동, 자전 행렬을 부모로 둔다.
	glm::mat4 Scale = glm::mat4(1.f);

	glm::mat4 RotateX = glm::mat4(1.f);
	glm::mat4 RotateY = glm::mat4(1.f);
	glm::mat4 RotateZ = glm::mat4(1.f);

	glm::mat4 Translate = glm::mat4(1.f);

	glm::mat4 RevolutionX = glm::mat4(1.f);
	glm::mat4 RevolutionY = glm::mat4(1.f);
	glm::mat4 RevolutionZ = glm::mat4(1.f);

	glm::mat4 Parent = glm::mat4(1.f);

	glm::mat4 Result = glm::mat4(1.f);

	Scale = glm::scale(Scale, _modelTransform.Scale);

	RotateX = glm::rotate(RotateX, glm::radians(_modelTransform.Rotate.x), glm::vec3(1.0, 0.0, 0.0));
	RotateY = glm::rotate(RotateY, glm::radians(_modelTransform.Rotate.y), glm::vec3(0.0, 1.0, 0.0));
	RotateZ = glm::rotate(RotateZ, glm::radians(_modelTransform.Rotate.z), glm::vec3(0.0, 0.0, 1.0));

	Translate = glm::translate(Translate, _modelTransform.Translate);

	RevolutionX = glm::rotate(RevolutionX, glm::radians(_modelTransform.Revolution.x), glm::vec3(1.0, 0.0, 0.0));
	RevolutionY = glm::rotate(RevolutionY, glm::radians(_modelTransform.Revolution.y), glm::vec3(0.0, 1.0, 0.0));
	RevolutionZ = glm::rotate(RevolutionZ, glm::radians(_modelTransform.Revolution.z), glm::vec3(0.0, 0.0, 1.0));
	if (_modelTransform.Parent) {
		Parent = glm::translate(Parent, _modelTransform.Parent->Translate);
		//Parent = glm::rotate(Parent, glm::radians(_modelTransform.Parent->Rotate.y), glm::vec3(0.0, 1.0, 0.0));
	}

	// 크 자 이 공 부 거꾸로
	Result = Parent * RevolutionX * RevolutionY * RevolutionZ * Translate * RotateX * RotateY * RotateZ * Scale;


	glUniformMatrix4fv(_modelLocation, 1, GL_FALSE, glm::value_ptr(Result));
}

void CShader_Func::Set_ArmModelTransform(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation)
{
	glm::mat4 Scale = glm::mat4(1.f);

	glm::mat4 RotateX = glm::mat4(1.f);
	glm::mat4 RotateY = glm::mat4(1.f);
	glm::mat4 RotateZ = glm::mat4(1.f);

	glm::mat4 Translate = glm::mat4(1.f);
	glm::mat4 PivotTranslate = glm::mat4(1.f);
	glm::mat4 RevolutionX = glm::mat4(1.f);
	glm::mat4 RevolutionY = glm::mat4(1.f);
	glm::mat4 RevolutionZ = glm::mat4(1.f);

	glm::mat4 Parent = glm::mat4(1.f);

	glm::mat4 Result = glm::mat4(1.f);

	Scale = glm::scale(Scale, _modelTransform.Scale);

	RotateX = glm::rotate(RotateX, glm::radians(_modelTransform.Rotate.x), glm::vec3(1.0, 0.0, 0.0));
	RotateY = glm::rotate(RotateY, glm::radians(_modelTransform.Rotate.y), glm::vec3(0.0, 1.0, 0.0));
	RotateZ = glm::rotate(RotateZ, glm::radians(_modelTransform.Rotate.z), glm::vec3(0.0, 0.0, 1.0));

	Translate = glm::translate(Translate, _modelTransform.Translate);
	PivotTranslate = glm::translate(Translate, _modelTransform.pivot);
	RevolutionX = glm::rotate(RevolutionX, glm::radians(_modelTransform.Revolution.x), glm::vec3(1.0, 0.0, 0.0));
	RevolutionY = glm::rotate(RevolutionY, glm::radians(_modelTransform.Revolution.y), glm::vec3(0.0, 1.0, 0.0));
	RevolutionZ = glm::rotate(RevolutionZ, glm::radians(_modelTransform.Revolution.z), glm::vec3(0.0, 0.0, 1.0));
	if (_modelTransform.Parent) {
		Parent = glm::translate(Parent, _modelTransform.Parent->Translate);
		Parent = glm::rotate(Parent, glm::radians(_modelTransform.Parent->Rotate.y), glm::vec3(0.0, 1.0, 0.0));
	}

	// 크 자 이 공 부 거꾸로
	Result = Parent * RevolutionX * RevolutionY * RevolutionZ * Translate * RotateX * RotateY * RotateZ * PivotTranslate * Scale;


	glUniformMatrix4fv(_modelLocation, 1, GL_FALSE, glm::value_ptr(Result));
}

bool CShader_Func::loadObj(const char* path, vector<glm::vec3>& out_vertices, vector<glm::vec2>& out_uvs, vector<glm::vec3>& out_normals, vector<glm::vec3>& out_color, glm::vec3 _color)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		cout << "Impossible to open the file!\n";
		return false;
	}

	while (1) {
		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	for (unsigned int i = 0; i < vertexIndices.size(); ++i) {
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
		out_color.push_back(_color);
	}
	fclose(file);

	return true;

}

GLubyte* CShader_Func::LoadDIBitmap(const char* filename, BITMAPINFO** info)
{
	FILE* fp;
	GLubyte* bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	//--- 바이너리 읽기 모드로 파일을 연다
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	//--- 비트맵 파일 헤더를 읽는다.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp); return NULL;
	}
	//--- 파일이 BMP 파일인지 확인한다.
	if (header.bfType != 'MB') {
		fclose(fp); return NULL;
	}
	//--- BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	//--- 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ((*info = (BITMAPINFO*)malloc(infosize)) == NULL) {
		fclose(fp); return NULL;
	}

	//--- 비트맵 인포 헤더를 읽는다.
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp); return NULL;
	}
	//--- 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth *
			(*info)->bmiHeader.biBitCount + 7) / 8.0 *
		abs((*info)->bmiHeader.biHeight);
	//--- 비트맵의 크기만큼 메모리를 할당한다.
	if ((bits = (unsigned char*)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp); return NULL;
	}
	//--- 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp); return NULL;
	}
	fclose(fp);
	return bits;
}

GLuint CShader_Func::loadBMP_custom(const char* imagepath)
{
	cout << "Reading image " << imagepath << endl;

	// BMP파일의 헤더에서 데이터를 읽어온다.
	unsigned char header[54];
	unsigned int dataPos; 
	unsigned int imageSize;
	unsigned int width, height;

	// 실제 RGB 데이터
	unsigned char* data;

	// 파일 개방
	FILE* file = fopen(imagepath, "rb");

	if (!file) {
		cout << imagepath << " could not be opened." << endl;
		getchar();
		return 0;
	}

	// 헤더 읽기 54 바이트 먼저
	// 54 보다 작으면 문제
	if (fread(header, 1, 54, file) != 54) {
		cout << "Not a correct BMP file" << endl;
		fclose(file);
		return 0;
	}

	// BMP 파일은 항상 "BM"으로 시작한다.
	if (header[0] != 'B' || header[1] != 'M') {
		cout << "Not Correct BMP file" << endl;
		fclose(file);
		return 0;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    fclose(file); return 0; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    fclose(file); return 0; }

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file can be closed.
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete[] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// ... which requires mipmaps. Generate them automatically.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;


}

void CShader_Func::Rotate(glm::vec3 _RotSize)
{
	cube.model_transform.Rotate += _RotSize;
}

void CShader_Func::Change_Shape()
{
	glm::vec3 tmp_Rotate = cube.model_transform.Rotate;

	cube = {};

	if (6 == polygonNum) {

		bool res = loadObj("pyramid.obj", cube.vertices, cube.uvs, cube.normals, cube.color, glm::vec3(1.f, 0.f, 0.f));
		cube.model_transform.Rotate = tmp_Rotate;
		polygonNum = 4;
		Update_Buffer(0, cube.vertices, cube.uvs);
	}
	else {
		bool res = loadObj("cube1212.obj", cube.vertices, cube.uvs, cube.normals, cube.color, glm::vec3(1.f, 0.f, 0.f));
		cube.model_transform.Rotate = tmp_Rotate;
		polygonNum = 6;
		Update_Buffer(0, cube.vertices, cube.uvs);
	}

}
 