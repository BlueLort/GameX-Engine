#include "pch.h"
#include "GLShaderManager.h"
namespace gx {
//Default Color Shader
const char* GLDefaultColorShader[] = {
//Vertex Shader
R"( 
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 vp;
void main()
{
    gl_Position = vp*model * vec4(aPos, 1.0);
}

)"
,
//Fragment Shader
R"( 
#version 330 core
out vec4 FragColor;
uniform vec3 col;
void main()
{
    FragColor = vec4(col, 1.0f);
} 
)"
,
//Geometry Shader
nullptr
	};
//Default Object Lighting Shader
const char* GLDefaultLightingShader[] = {
	//Shader Example Logic from learnopengl.com [with some modifications]
	//Vertex Shader
	R"( 
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 vp;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = vp * vec4(FragPos, 1.0);
}
)"
,
//Fragment Shader
R"( 
#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse1;
	sampler2D diffuse2;
	sampler2D diffuse3;
    sampler2D specular1;
	sampler2D specular2;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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

#define NR_POINT_LIGHTS 1

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

vec3 totalDiffuse;
vec3 totalSpecular;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    

	totalDiffuse=vec3(texture(material.diffuse1,TexCoords)+texture(material.diffuse2,TexCoords)+texture(material.diffuse3,TexCoords));
	totalSpecular=vec3(texture(material.specular1,TexCoords)+texture(material.specular2,TexCoords));

    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // phase 3: spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    //FragColor = vec4(result,1.0);
	FragColor = vec4(totalDiffuse,1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * totalDiffuse;
    vec3 diffuse = light.diffuse * diff * totalDiffuse;
    vec3 specular = light.specular * spec * totalSpecular;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * totalDiffuse;
    vec3 diffuse = light.diffuse * diff * totalDiffuse;
    vec3 specular = light.specular * spec * totalSpecular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * totalDiffuse;
    vec3 diffuse = light.diffuse * totalDiffuse;
    vec3 specular = light.specular * spec * totalSpecular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
} 
)"
,
//Geometry Shader
nullptr
};
//Default Model Shader
const char* GLDefaultModelShader[] = {
	//Vertex Shader
	R"( 
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 vp;
void main()
{
     FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = vp * vec4(FragPos, 1.0);
}

)"
,
//Fragment Shader
R"( 
#version 330 core
out vec4 FragColor;

uniform sampler2D texture_diffuse;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
    FragColor = texture(texture_diffuse,TexCoords);
} 
)"
,
//Geometry Shader
nullptr
};
}