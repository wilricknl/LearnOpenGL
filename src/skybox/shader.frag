#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	// refraction
	float ratio = 1.00f / 1.52f; // glass

	vec3 I = normalize(Position - cameraPos);
	vec3 R = refract(I, normalize(Normal), ratio);
	FragColor = vec4(texture(skybox, R).rgb, 1.0);

	// default
	//FragColor = texture(texture_diffuse1, TexCoords);
}
