#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	// resulting position of the vertex
	gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);

	// Pass location of the fragment in world space
	FragPos = vec3(model * vec4(aPos, 1.0));

	// Do funky stuff with normal vector and pass it
	Normal = mat3(transpose(inverse(model))) * aNormal;

	// Pass texture coordinates
	TexCoord = aTexCoord;
}
