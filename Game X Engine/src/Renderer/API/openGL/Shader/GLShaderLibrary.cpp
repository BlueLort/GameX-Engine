#include "pch.h"
#include "GLShaderManager.h"
namespace gx {

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
//Geometry Shader
nullptr
,
//Fragment Shader
R"( 
#version 430 core
out vec4 FragColor;
in vec3 FragPos;
void main()
{
	const vec3 baseColor = vec3(0.2,0.5,0.92);
	float factor = FragPos.y - 700;
	float r= -0.0002*factor+baseColor.r;
	float g= -0.00025*factor+baseColor.g;
	float b= -0.0005*factor+baseColor.b;
    FragColor = vec4(r,g,b,1.0);
} 
)"

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
//Geometry Shader
nullptr
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

	};
	const char* GLDefaultGBufferShader[] = {
	R"( 
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 vp;
uniform mat4 lightSpaceMatrix;
uniform sampler2D normalMap;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz; 
    TexCoords = aTexCoords;
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 N = normalize(normalMatrix * aNormal);
    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    //re-orthogonalize T with respect to Normal [Gramm-Schimdt Process]
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T, B, N);
    vec3 normal = texture(normalMap, TexCoords).rgb;
    normal = normal * 2.0 - 1.0;   
    Normal = normalize(TBN * normal);

    FragPosLightSpace = lightSpaceMatrix * worldPos;
    gl_Position = vp * worldPos;
}
)"
,
//Geometry Shader
nullptr
,
//Fragment Shader
R"( 
#version 430 core
layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec3 gPosition;
layout (location = 2) out vec3 gNormal;
layout (location = 3) out uint gID;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D displacement;
    float shininess;
};  

vec3 totalDiffuse;
vec3 totalSpecular;

uniform Material material;
uniform uint objID = 0;//let 0 be initial value for this as it's used for skydome/skybox objects
uniform sampler2D dirLightDepthMap;
uniform vec3 lightPos;
float calculateShadow(vec4 fragPosLightSpace);
void main()
{    
	totalDiffuse = texture(material.diffuse,TexCoords).rgb;
	totalSpecular = texture(material.specular,TexCoords).rgb;
    gPosition = FragPos;
    gNormal = normalize(Normal);
    float shadow = calculateShadow(FragPosLightSpace); 

    vec3 ambient  = totalDiffuse.rgb * 0.1;// hard-coded ambient component
    gAlbedoSpec.rgb = ambient + (1.0 - shadow) * (totalDiffuse.rgb - ambient);
    gAlbedoSpec.a = totalSpecular.r;
	gID = objID;
	
}
float calculateShadow(vec4 fragPosLightSpace) {
    // perspective division
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    // closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(dirLightDepthMap, projCoords.xy).r; 
    // depth of current fragment from light's perspective
    float currentDepth = projCoords.z;


    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // check whether current frag pos is in shadow with PCF for softer shadows
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(dirLightDepthMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(dirLightDepthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}
)"

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
//Geometry Shader
nullptr
,
//Fragment Shader
R"( 
#version 430 core
out vec4 FragColor;
in vec2 TexCoords;


uniform sampler2D gAlbedoSpec;
uniform sampler2D gPosition;
uniform sampler2D gNormal;



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

	vec3 ambient  = Diffuse * 0.1;// hard-coded ambient component
    vec3 lighting  = ambient; 

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

	};
	//Default Outline Shader Shader
	const char* GLDefaultOutlineShader[] = {
		//Vertex Shader
		R"( 
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
uniform mat4 model;
uniform mat4 vp;
void main()
{
    gl_Position = vp * model * vec4( aPos + aNormal * 0.1, 1.0); //scale along the normals
}

)"
,
//Geometry Shader
nullptr
,
//Fragment Shader
R"( 
#version 430 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(0.95f,0.95f,0.95f,1.0f);
} 
)"

	};


	//Default Grid Shader Shader
	const char* GLDefaultGridShader[] = {
		//Vertex Shader
		R"( 
#version 430 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 vp;
void main()
{
    gl_Position = vp * model * vec4( aPos , 1.0);
}

)"
,
//Geometry Shader
nullptr
,
//Fragment Shader
R"( 
#version 430 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(0.2f,0.4f,0.2f,1.0f);
} 
)"

	};
	//Default Shadow Map Shader Shader
	const char* GLDefaultShadowMapShader[] = {
		//Vertex Shader
		R"( 
#version 430 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
)"
,
//Geometry Shader
nullptr
,
//Fragment Shader
R"( 
void main()
{             
    //gl_FragDepth = gl_FragCoord.z;
}  
)"
	};
}