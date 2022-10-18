#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;

  vec3 norm = normalize(Normal);
  vec3 lightDirection = normalize(lightPosition - FragPos);
  float difference = max(dot(norm, lightDirection), 0.0f);
  vec3 diffuse = difference * lightColor;

  float specularStrength = 0.5f;
  vec3 viewDirection = normalize(viewPosition - FragPos);
  vec3 reflectDirection = reflect(-lightDirection, norm);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32.0);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 light = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(light, 1.0f);
}