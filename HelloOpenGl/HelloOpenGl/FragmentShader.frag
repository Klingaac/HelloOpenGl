#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	//FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), sinValue) * (1);
	FragColor = vec4(lightColor * objectColor, 1);
}