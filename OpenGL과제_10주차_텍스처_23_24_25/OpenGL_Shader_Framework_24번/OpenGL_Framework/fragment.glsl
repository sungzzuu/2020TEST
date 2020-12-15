#version 330
//--- ex_Color: ���ؽ� ���̴����� �Է¹޴� ���� ��
//--- gl_FragColor: ����� ������ ������ ���� ���α׷����� ���� ��. 
in vec3 ex_Color; //--- ���ؽ� ���̴����Լ� ���� ����
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 gl_FragColor; //--- ���� ���

void main(void) 
{
	float ambientLight = 0.3;
	vec3 ambient = ambientLight * lightColor;

	vec3 normalVector = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseLight = max(dot(normalVector,lightDir),0.0);
	vec3 diffuse = diffuseLight * lightColor;

	//----- �ſ� ����
	int shininess = 128;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalVector);
	float specularLight = max(dot(viewDir,reflectDir), 0.0);
	specularLight = pow(specularLight, shininess);
	vec3 specular = specularLight * lightColor;


	vec3 result = (ambient + diffuse + specular) * ex_Color;

	gl_FragColor = vec4(result,0.5);
}