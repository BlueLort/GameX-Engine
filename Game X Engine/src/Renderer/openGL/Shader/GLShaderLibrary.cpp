#include "pch.h"
#include "GLShaderManager.h"
namespace gx {
//Default Shader
const char* GLDefaultShader[] = {
//Vertex Shader
R"( 
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 mvp;
void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
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

}