#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main()
{
	float ambientStrenght = 0.1;
	float range = 3;
	float specularStrenght = 0.5;

	vec3 ambient = ambientStrenght * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular light (shiny spot)
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrenght * spec * lightColor;  

	// Distance light falloff
	float dist = length(lightPos - FragPos);
	float attenuation = 1.0 / (1.0 + (dist / range) * (dist / range));

	// Result
	vec3 result = (ambientStrenght + (diffuse * attenuation) + (specular * attenuation)) * objectColor;
	FragColor = vec4(result, 1.0);
}