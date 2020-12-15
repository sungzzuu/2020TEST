#version 330

out vec4 gl_FragColor;

in vec2 TexCoord;

uniform sampler2D outTexture;

void main() 
{
	gl_FragColor = texture(outTexture, TexCoord);

	//--- 조명을 설정하는 경우:
	//--- FragColor = vec4 (result, 1.0f);
	//--- FragColor = texture(outTexture, TexCoord) * FragColor;
}