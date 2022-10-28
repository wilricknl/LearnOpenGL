#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 direction;
  // vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
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
  // ambient
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDirection = normalize(-light.direction);
  float difference = max(dot(norm, lightDirection), 0.0f);
  vec3 diffuse = light.diffuse * difference * vec3(texture(material.diffuse, TexCoords));

  // specular
  vec3 viewDirection = normalize(viewPosition - FragPos);
  vec3 reflectDirection = reflect(-lightDirection, norm);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
  vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

  vec3 result = ambient + diffuse + specular;
  FragColor = vec4(result, 1.0f);
}
