#version 330

out vec4 FragColor;

uniform vec3 sourceColor;

void main()
{
	// Lightsource should be always full white
	FragColor = vec4(sourceColor, 1.0f);
}