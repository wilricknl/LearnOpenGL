#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;

out vec3 light;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0f);

  vec3 FragPos = vec3(view * model * vec4(aPos, 1.0f));
  vec3 Normal = mat3(transpose(inverse(view * model))) * aNormal;
  vec3 LightPos = vec3(view * vec4(lightPosition, 1.0f));

  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;

  vec3 norm = normalize(Normal);
  vec3 lightDirection = normalize(LightPos - FragPos);
  float difference = max(dot(norm, lightDirection), 0.0f);
  vec3 diffuse = difference * lightColor;

  float specularStrength = 0.5f;
  vec3 viewDirection = normalize(-FragPos);
  vec3 reflectDirection = reflect(-lightDirection, norm);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32.0);
  vec3 specular = specularStrength * spec * lightColor;

  light = (ambient + diffuse + specular) * objectColor;
}
