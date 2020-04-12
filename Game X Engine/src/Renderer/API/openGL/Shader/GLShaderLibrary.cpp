#include "pch.h"
#include "GLShaderManager.h"
namespace gx {
	//Default Color Shader
	const char* GLDefaultColorShader[] = {
		//Vertex Shader
		R"( 
#version 430 core
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
#version 430 core
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
#version 430 core
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
#version 430 core
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
   // totalDiffuse=vec3(1.0,1.0,1.0); 
	//totalSpecular=vec3(0.4,0.4,0.4);
	// properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights
    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
      //  result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // phase 3: spot light
   // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    FragColor = vec4(result,1.0);
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
#version 430 core
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
#version 430 core
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

	const char* GLDefaultSkydomeShader[] = {
		//Vertex Shader
		R"( 
#version 430 core
layout (location = 0) in vec3 aPos;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	 vec4 worldPos=model*vec4(aPos,1.0);
     FragPos = vec3(worldPos);
	 vec4 PVPos=projection* mat4(mat3(view)) * worldPos;
	
	 gl_Position = PVPos.xyww;
}
)"
,
//Fragment Shader
R"( 
#version 430 core
out vec4 FragColor;
in vec3 FragPos;
void main()
{
	const vec3 baseColor=vec3(0.203,0.596,0.921);
	float factor=FragPos.y-1000;
	float r= -0.0002*factor+baseColor.r;
	float g= -0.00022*factor+baseColor.g;
	float b= -0.0001*factor+baseColor.b;
    FragColor = vec4(r,g,b,1.0);
} 
)"
,
//Geometry Shader
nullptr
	};
	const char* GLDefaultPlaneShader[] = {
		R"( 
#version 430 core
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
#version 430 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse1;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;

vec3 totalDiffuse;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
void main()
{    

	totalDiffuse=vec3(texture(material.diffuse1,TexCoords));
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
    FragColor = vec4(result,1.0);
}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambient = light.ambient * totalDiffuse;
    vec3 diffuse = light.diffuse * diff * totalDiffuse;
    return (ambient + diffuse);
}
)"
,
//Geometry Shader
nullptr
	};
	const char* GLDefaultGBufferShader[] = {
	R"( 
#version 430 core
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
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz; 
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalMatrix * aNormal;

    gl_Position = vp * worldPos;
}
)"
,
//Fragment Shader
R"( 
#version 430 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out uint gID;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
struct Material {
    sampler2D diffuse1;
	sampler2D diffuse2;
	sampler2D diffuse3;
    sampler2D specular1;
	sampler2D specular2;
    float shininess;
};  

vec3 totalDiffuse;
vec3 totalSpecular;

uniform Material material;
uniform uint objID = 0;//let 0 be initial value for this as it's used for skydome/skybox objects

void main()
{    
	totalDiffuse=vec3(texture(material.diffuse1,TexCoords)+texture(material.diffuse2,TexCoords)+texture(material.diffuse3,TexCoords));
	totalSpecular=vec3(texture(material.specular1,TexCoords)+texture(material.specular2,TexCoords));
    gPosition = FragPos;
    gNormal = normalize(Normal);
    gAlbedoSpec.rgb = totalDiffuse.rgb;
    gAlbedoSpec.a = totalSpecular.r;
	gID = objID;
	
}
)"
,
//Geometry Shader
nullptr
	};

	const char* GLDefaultDeferredShader[] = {
		R"( 
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}
)"
,
//Fragment Shader
R"( 
#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

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
const int NR_LIGHTS = 1;
uniform DirLight dirLight;
uniform PointLight lights[NR_LIGHTS];
uniform vec3 viewPos;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir,vec3 color);
void main()
{             
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

	
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component

    vec3 viewDir  = normalize(viewPos - FragPos);
	vec3 result = CalcDirLight(dirLight, Normal, viewDir , Diffuse);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].diffuse;

        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = lights[i].specular * spec * Specular;

        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (1.0 + lights[i].linear * distance + lights[i].quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;        
    }
	result += lighting;
    FragColor = vec4(result, 1.0);
}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir ,vec3 color)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff * color;
    return (ambient + diffuse);
}
)"
,
//Geometry Shader
nullptr
	};
}