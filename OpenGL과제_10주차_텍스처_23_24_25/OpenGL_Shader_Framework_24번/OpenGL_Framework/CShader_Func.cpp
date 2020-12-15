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
	
	// ============================= 고정 ============================================
	glLineWidth(1.f);
	glPolygonMode(GL_FRONT_AND_BACK, ePolygonMode);
	glEnable(GL_CULL_FACE);
	// 사용할 VAO 불러오기
	glUseProgram(s_program);
	// s_program 셰이더 사용 모델변환
	unsigned int modelLocation = glGetUniformLocation(s_program, "model");
	unsigned int viewLocation = glGetUniformLocation(s_program, "view");
	unsigned int projLocation = glGetUniformLocation(s_program, "projection");
	// ============================= 고정 ============================================
	
	
	// ============================= 뷰 변환 ============================================
	// 카메라 좌표 설정
	glm::vec3 cameraPos = Camera_Pos;
	glm::vec3 cameraDirection = m_Camera_Desc.cameraDirection;
	glm::vec3 cameraUp = m_Camera_Desc.cameraUp;
	

	

	glm::mat4 view = glm::mat4(1.f);
	glm::mat4 view_Rotate = glm::mat4(1.f);
	view_Rotate = glm::rotate(view_Rotate, glm::radians(m_Camera_Rotate_Angle), glm::vec3(0.0, 1.0, 0.0));

	view = view_Rotate * glm::lookAt(cameraPos, cameraDirection, cameraUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	// ============================= 뷰 변환 ============================================

	// ============================= 투영 변환 ============================================
	glm::mat4 projection = glm::mat4(1.f);
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
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	int lightPosLocation = glGetUniformLocation(s_program, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, light.model_transform.Translate.x, light.model_transform.Translate.y, light.model_transform.Translate.z);
	unsigned int LightColorLocation = glGetUniformLocation(s_program, "lightColor");
	glUniform3f(LightColorLocation, Light_Color.x, Light_Color.y, Light_Color.z);
	unsigned int viewPosLocation = glGetUniformLocation(s_program, "viewPos");
	glUniform3f(viewPosLocation, Camera_Pos.x, Camera_Pos.y, Camera_Pos.z);
	int vIndex = 0;


	

	Set_ModelTransform(bottom.model_transform, modelLocation);
	glBindVertexArray(vao[vIndex++]);
	glDrawArrays(GL_TRIANGLES, 0, bottom.vertices.size());

	Set_ModelTransform(pyramid.model_transform, modelLocation);
	glBindVertexArray(vao[vIndex++]);
	glDrawArrays(GL_TRIANGLES, 0, pyramid.vertices.size());

	for (int i = 0; i < 3; ++i) {
		Set_ModelTransform(sphere[i].model_transform, modelLocation);
		glBindVertexArray(vao[vIndex++]);
		glDrawArrays(GL_TRIANGLES, 0, sphere[i].vertices.size());
	}
	


	for (auto& snowball : snow) {
		Set_ModelTransform(snowball.model_transform, modelLocation);
		glBindVertexArray(vao[vIndex++]);
		glDrawArrays(GL_TRIANGLES, 0, snowball.vertices.size());
	}
	

	for (auto& snowbuild : snow_build) {
		Set_ModelTransform(snowbuild->model_transform, modelLocation);
		glBindVertexArray(vao[vIndex++]);
		glDrawArrays(GL_TRIANGLES, 0, snowbuild->vertices.size());
	}
	Set_ModelTransform(light.model_transform, modelLocation);
	glBindVertexArray(vao[vIndex++]);
	glDrawArrays(GL_TRIANGLES, 0, light.vertices.size());

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (auto& column : columns) {
		Set_ModelTransform_Column(column.model_transform, modelLocation);
		glBindVertexArray(vao[vIndex++]);
		glDrawArrays(GL_TRIANGLES, 0, column.vertices.size());

	}
	glDisable(GL_BLEND);

}


void CShader_Func::Initialize_Shader()
{
	
	make_vertexShader();
	make_fragmentShader();

	for (int i = 0; i < 3; ++i) {
		bool res = loadObj("sphere.obj", sphere[i].vertices, sphere[i].uvs, sphere[i].normals, sphere[i].color, {1.f, 0.f+ i*0.5f,0.f});
	}
	bool res = loadObj("sphere.obj", light.vertices, light.uvs, light.normals, light.color, {1.f, 1.f, 1.f});
	res = loadObj("Cube.obj", bottom.vertices, bottom.uvs, bottom.normals, bottom.color, { 0.8, 0.6, 0.4 });
	res = loadObj("pyramid.obj", pyramid.vertices, pyramid.uvs, pyramid.normals, pyramid.color, { 1.0, 1.0, 0.0 });
	for (auto& snowball : snow) {
		res = loadObj("snow.obj", snowball.vertices, snowball.uvs, snowball.normals, snowball.color, { 1.0, 1.0, 1.0 });
	}
	for(auto& column :columns)
		loadObj_RGBA("Cube.obj", column.vertices, column.uvs, column.normals, column.color_rgba, { 0.0, 0.0, 0.8, 0.2 });
	Init_Player();
	
	// 젠버퍼 여기서
	for (int i = 0; i < object_num; ++i) {
		glGenVertexArrays(i, &vao[i]); //--- VAO 를 지정하고 할당하기
		glGenBuffers(2, vbo[i]); //--- 2개의 VBO를 지정하고 할당하기
		glGenBuffers(1, &normalbuffer);
	}
	
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

void CShader_Func::Init_Player()
{
	bottom.model_transform.Scale.y = 0.01f;
	bottom.model_transform.Scale.x = 6.f;
	bottom.model_transform.Scale.z = 6.f;
	bottom.model_transform.Translate = glm::vec3(0.f, 0.f, 0.f);
	sphere[0].model_transform.Scale = glm::vec3(0.01f, 0.01f, 0.01f);
	sphere[1].model_transform.Scale = glm::vec3(0.008f, 0.008f, 0.008f);
	sphere[2].model_transform.Scale = glm::vec3(0.005f, 0.005f, 0.005f);

	sphere[0].model_transform.Translate = glm::vec3(-1.f, 2.f, 0.f);
	sphere[1].model_transform.Translate = glm::vec3(-2.f, 2.f, -1.f);
	sphere[2].model_transform.Translate = glm::vec3(-3.f, 2.f, -2.f);

	light.model_transform.Scale = glm::vec3(0.008f, 0.008f, 0.008f);
	light.model_transform.Translate = glm::vec3(2.f, 10.f, 5.f);

	pyramid.model_transform.Scale = glm::vec3(0.2f, 0.2f, 0.2f);
	pyramid.model_transform.Rotate = glm::vec3(0.f, -30.f, 0.f);

	for (auto& snowball : snow) {
		snowball.model_transform.Scale = glm::vec3(0.05f, 0.05f, 0.05f);
		snowball.model_transform.Translate = glm::vec3(rand()%1000*0.001f*6.f-3.f, 5.f, rand() % 1000* 0.001f * 6.f - 3.f);
		snowball.fSpeed = rand() % 100 * 0.001f + 0.001f;
	}
	int cnt = 0;
	for (auto& column : columns) {
		column.model_transform.Scale = { 1.f,3.f,1.f };
		column.model_transform.Parent = &pyramid.model_transform;
	}
	columns[0].model_transform.Translate = { -5.f, 0.f,-5.f };
	columns[1].model_transform.Translate = { 0.f, 0.f,-5.f };
	columns[2].model_transform.Translate = { 5.f, 0.f,-5.f };
	columns[3].model_transform.Translate = { -5.f, 0.f, 5.f };
	columns[4].model_transform.Translate = { 0.f, 0.f, 5.f };
	columns[5].model_transform.Translate = { 5.f, 0.f, 5.f };

	m_Camera_Desc.cameraDirection = glm::vec3(0.f, 3.f, 0.f);
}

void CShader_Func::Update()
{

	int vIndex = 0;

	Update_Buffer(vIndex++, bottom.vertices, bottom.normals, bottom.color);
	Update_Buffer(vIndex++, pyramid.vertices, pyramid.normals, pyramid.color);

	for (int i = 0; i < 3; ++i) {
		Update_Buffer(vIndex++, sphere[i].vertices, sphere[i].normals, sphere[i].color);
	}
	for (auto& snowball : snow) {
		Update_Buffer(vIndex++, snowball.vertices, snowball.normals, snowball.color);

	}
	for (auto& snowbuid : snow_build) {
		Update_Buffer(vIndex++, snowbuid->vertices, snowbuid->normals, snowbuid->color);

	}
	Update_Buffer(vIndex++, light.vertices, light.normals, light.color);

	for (auto& column : columns) {
		Update_Buffer_column(vIndex++, column.vertices, column.normals, column.color_rgba);

	}
	Update_Shader();

	Call_Vao();

}

void CShader_Func::Update_Buffer(int _vindex, vector<glm::vec3> _vertices, vector<glm::vec3> _normals, vector<glm::vec3> _color)
{
	//--- VAO 객체 생성 및 바인딩
	glBindVertexArray(vao[_vindex]); //--- VAO를 바인드하기

	//--- vertex data 저장을 위한 VBO 생성 및 바인딩
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[_vindex][0]);
	// 변수 triShape 에서 버텍스 데이터 값을 버퍼에 복사한다.

	// triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);

	
	
	//--- 색상 속성
	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[_vindex][1]);
	// 변수 colors에서 버텍스 색상을 복사한다.
	// colors 배열의 사이즈: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, _color.size() * sizeof(glm::vec3), &_color[0], GL_STATIC_DRAW);
	// 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);


	// normal
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(glm::vec3), &_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);

}

void CShader_Func::Update_Buffer_column(int _vindex, vector<glm::vec3> _vertices, vector<glm::vec3> _normals, vector<glm::vec4> _color)
{

	//--- VAO 객체 생성 및 바인딩
	glBindVertexArray(vao[_vindex]); //--- VAO를 바인드하기

	//--- vertex data 저장을 위한 VBO 생성 및 바인딩
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[_vindex][0]);
	// 변수 triShape 에서 버텍스 데이터 값을 버퍼에 복사한다.

	// triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);



	//--- 색상 속성
	// 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	// attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[_vindex][1]);
	// 변수 colors에서 버텍스 색상을 복사한다.
	// colors 배열의 사이즈: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, _color.size() * sizeof(glm::vec4), &_color[0], GL_STATIC_DRAW);
	// 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	// attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);


	// normal
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(glm::vec3), &_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
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
	glUseProgram(s_program);
}


void CShader_Func::Set_ModelTransform(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation)
{
	glm::mat4 Scale = glm::mat4(1.f);
	
	glm::mat4 RotateX = glm::mat4(1.f);
	glm::mat4 RotateY = glm::mat4(1.f);
	glm::mat4 RotateZ = glm::mat4(1.f);

	glm::mat4 Translate = glm::mat4(1.f);
	glm::mat4 pivotTranslate = glm::mat4(1.f);

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
	pivotTranslate = glm::translate(pivotTranslate, _modelTransform.pivot);

	RevolutionX = glm::rotate(RevolutionX, glm::radians(_modelTransform.Revolution.x), glm::vec3(1.0, 0.0, 0.0));
	RevolutionY = glm::rotate(RevolutionY, glm::radians(_modelTransform.Revolution.y), glm::vec3(0.0, 1.0, 0.0));
	RevolutionZ = glm::rotate(RevolutionZ, glm::radians(_modelTransform.Revolution.z), glm::vec3(0.0, 0.0, 1.0));
	if(_modelTransform.Parent)
		Parent = glm::translate(Parent, _modelTransform.Parent->Translate);

	// 크 자 이 공 부 거꾸로
	Result = Parent * RevolutionX * RevolutionY * RevolutionZ * Translate * RotateX * RotateY * RotateZ * pivotTranslate *Scale;


	glUniformMatrix4fv(_modelLocation, 1, GL_FALSE, glm::value_ptr(Result));
}

void CShader_Func::Set_ModelTransform_Column(MODEL_TRANSFORM _modelTransform, unsigned int _modelLocation)
{
	glm::mat4 Scale = glm::mat4(1.f);

	glm::mat4 RotateX = glm::mat4(1.f);
	glm::mat4 RotateY = glm::mat4(1.f);
	glm::mat4 RotateZ = glm::mat4(1.f);

	glm::mat4 Translate = glm::mat4(1.f);
	glm::mat4 pivotTranslate = glm::mat4(1.f);

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
	pivotTranslate = glm::translate(pivotTranslate, _modelTransform.pivot);

	RevolutionX = glm::rotate(RevolutionX, glm::radians(_modelTransform.Revolution.x), glm::vec3(1.0, 0.0, 0.0));
	RevolutionY = glm::rotate(RevolutionY, glm::radians(_modelTransform.Revolution.y), glm::vec3(0.0, 1.0, 0.0));
	RevolutionZ = glm::rotate(RevolutionZ, glm::radians(_modelTransform.Revolution.z), glm::vec3(0.0, 0.0, 1.0));
	if (_modelTransform.Parent) {
		Parent = glm::translate(Parent, glm::vec3(0.f,0.f,-12.f));
		Parent = glm::rotate(Parent, glm::radians(_modelTransform.Parent->Rotate.y), glm::vec3(0.0, 1.0, 0.0));
	}

	// 크 자 이 공 부 거꾸로
	Result = Parent * RevolutionX * RevolutionY * RevolutionZ * Translate * RotateX * RotateY * RotateZ * Scale;


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
	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
		out_color.push_back(_color);

	}

	object_num++;
}

bool CShader_Func::loadObj_RGBA(const char* path, vector<glm::vec3>& out_vertices, vector<glm::vec2>& out_uvs, vector<glm::vec3>& out_normals, vector<glm::vec4>& out_color_rgba, glm::vec4 _color)
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
	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
		out_color_rgba.push_back(_color);

	}

	object_num++;
}

void CShader_Func::Move_LightPos(glm::vec3 _MoveSize)
{
	Light_Rotate.z += 0.05f;
	light.model_transform.Translate.y -=sin(Light_Rotate.z);
	light.model_transform.Translate.x += cos(Light_Rotate.z);

}

void CShader_Func::Change_LightColor(glm::vec3 _color)
{
	Light_Color = _color;
}

void CShader_Func::Move_Camera(glm::vec3 _moveSize)
{
	Camera_Pos += _moveSize * 0.1f;
	m_Camera_Desc.cameraDirection += _moveSize * 0.1f;
}

void CShader_Func::Recolution_Planets()
{
	sphere[0].model_transform.Revolution.y += 1.f;
	sphere[1].model_transform.Revolution.y += 1.5f;
	sphere[2].model_transform.Revolution.y += 2.f;

}

void CShader_Func::snow_animation()
{
	// 5.f 가 상층 0.f까지 떨어진다. 다시 5.f로 복귀
	for (auto& snowball : snow) {
		if (snowball.model_transform.Translate.y < 0.f) {
			snowball.model_transform.Translate.y = 5.f;
			buildCnt++;
			if (buildCnt > 10) {
				buildCnt = 0;
				Init_build_snow();
			}
			// 랜덤한 위치에 y값이 0이고 납작한 눈 그리기
		}
		snowball.model_transform.Translate.y -= snowball.fSpeed;
	}
}

void CShader_Func::Init_build_snow()
{
	if (snow_build.size() > 50) {
		snow_build.pop_front();
	}
	// 1. 리스트에 넣기
	OBJECT* pObject = new OBJECT;
	pObject->model_transform.Scale = glm::vec3(0.05f, 0.001f, 0.05f);
	pObject->model_transform.Translate = glm::vec3(rand() % 1000 * 0.001f * 6.f - 3.f, 0.f, rand() % 1000 * 0.001f * 6.f - 3.f);
	snow_build.push_back(pObject);

	// 2. 로드 & 젠버퍼
	bool res = loadObj("snow.obj", pObject->vertices, pObject->uvs, pObject->normals, pObject->color, { 0.8, 0.8, 0.8 });
	
	glGenVertexArrays(object_num-1, &vao[object_num-1]); //--- VAO 를 지정하고 할당하기
	glGenBuffers(2, vbo[object_num-1]); //--- 2개의 VBO를 지정하고 할당하기
	glGenBuffers(1, &normalbuffer);

}

void CShader_Func::Rotate_All_Objects()
{
	for (int i = 0; i < 3; ++i)
		sphere[i].model_transform.Rotate.y += 1.f;
	bottom.model_transform.Rotate.y += 1.f;
	pyramid.model_transform.Rotate.y += 1.f;

	for(int i = 0; i < 30; ++i)
		snow[i].model_transform.Rotate.y += 1.f;

	
}
