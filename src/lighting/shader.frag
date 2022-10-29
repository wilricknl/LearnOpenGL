#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection, vec3 fragPosition);

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection, vec3 fragPosition);

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform DirectionalLight directionalLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 viewPosition;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDirection = normalize(viewPosition - FragPos);
	
	vec3 result = CalculateDirectionalLight(directionalLight, normal, viewDirection);
	
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalculatePointLight(pointLights[i], normal, viewDirection, FragPos);
	}
	
	result += CalculateSpotLight(spotLight, normal, viewDirection, FragPos);

	FragColor = vec4(result, 1.0f);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction);
	float difference = max(dot(normal, lightDirection), 0.0f);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float shininess = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * difference * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * shininess * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection, vec3 fragPosition)
{
	vec3 lightDirection = normalize(light.position - fragPosition);
	float difference = max(dot(normal, lightDirection), 0.0f);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float shininess = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);

	float distance = length(light.position - fragPosition);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * difference * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * shininess * vec3(texture(material.specular, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection, vec3 fragPosition)
{
	vec3 lightDirection = normalize(light.position - fragPosition);
	float difference = max(dot(normal, lightDirection), 0.0f);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float shininess = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * difference * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * shininess * vec3(texture(material.specular, TexCoords));

	// spotlight soft edges
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = (light.cutOff - light.outerCutOff);
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);
	diffuse *= intensity;
	specular *= intensity;

	// attenuation
	float distance = length(light.position - fragPosition);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}
