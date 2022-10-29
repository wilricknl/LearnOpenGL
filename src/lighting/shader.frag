#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPosition;

void main()
{
	vec3 lightDirection = normalize(light.position - FragPos);
  
	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// diffuse
	vec3 norm = normalize(Normal);
	float difference = max(dot(norm, lightDirection), 0.0f);
	vec3 diffuse = light.diffuse * difference * vec3(texture(material.diffuse, TexCoords));

	// specular
	vec3 viewDirection = normalize(viewPosition - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

	// spotlight soft edges
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
	diffuse *= intensity;
	specular *= intensity;

	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}
