#pragma once
#include "Config.h"
#ifdef USING_OPENGL
	#include "Renderer/API/openGL/GLLoader/glad.h"
#endif
namespace gx {
	enum GXAttachmentType {
		GX_COLOR_TEXTURE = 0, // Albedo/Color Attachment
		GX_DEPTH_TEXTURE,
		GX_POSITION_TEXTURE,
		GX_NORMAL_TEXTURE,
		GX_ID_TEXTURE,
		GX_STENCIL_TEXTURE

	};
#ifdef USING_OPENGL
	enum RenderType {
		GX_TRIANGLES = GL_TRIANGLES,
		GX_TRIANGLES_STRIP = GL_TRIANGLE_STRIP,
		GX_TRIANGLES_FAN = GL_TRIANGLE_FAN,
		GX_TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
		GX_TRIANGLES_STRIP_ADJACENCY = GL_TRIANGLES_ADJACENCY
	};
	enum DepthFunc {
		GX_NEVER = GL_NEVER,
		GX_LESS = GL_LESS,
		GX_EQUAL = GL_EQUAL,
		GX_LEQUAL = GL_LEQUAL,
		GX_GREATER = GL_GREATER,
		GX_GEQUAL = GL_GEQUAL,
		GX_NOTEQUAL = GL_NOTEQUAL,
		GX_ALWAYS = GL_ALWAYS
	};
	enum WindingOrder {
		GX_CCW = GL_CCW,
		GX_CW = GL_CW,
	};
	enum GXEnumType {
		GX_FLOAT = GL_FLOAT,
		GX_RED_INTEGER = GL_RED_INTEGER,
		GX_UNSIGNED_INT = GL_UNSIGNED_INT
		//addmore when needed
	};
	enum GXRendererFlag {
		GX_CULL_FACE = GL_CULL_FACE,
		GX_DEPTH_TEST = GL_DEPTH_TEST
	};
	enum GXBufferBits {
		GX_COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
		GX_DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
		GX_STENCIL_BUFFER_BIT = GL_STENCIL_BUFFER_BIT
	};
	enum GXAttachmentIndex {
		GX_NONE_ATTACHMENT = GL_NONE,
		GX_COLOR_ATTACHMENT0 = GL_COLOR_ATTACHMENT0,
		GX_COLOR_ATTACHMENT1 = GL_COLOR_ATTACHMENT1,
		GX_COLOR_ATTACHMENT2 = GL_COLOR_ATTACHMENT2,
		GX_COLOR_ATTACHMENT3 = GL_COLOR_ATTACHMENT3,
		GX_COLOR_ATTACHMENT4 = GL_COLOR_ATTACHMENT4,
		GX_COLOR_ATTACHMENT5 = GL_COLOR_ATTACHMENT5,
		GX_COLOR_ATTACHMENT6 = GL_COLOR_ATTACHMENT6,
		GX_COLOR_ATTACHMENT7 = GL_COLOR_ATTACHMENT7,
		GX_COLOR_ATTACHMENT8 = GL_COLOR_ATTACHMENT8,
		GX_COLOR_ATTACHMENT9 = GL_COLOR_ATTACHMENT9,
		GX_COLOR_ATTACHMENT10 = GL_COLOR_ATTACHMENT10,
		GX_COLOR_ATTACHMENT11 = GL_COLOR_ATTACHMENT11,
		GX_COLOR_ATTACHMENT12 = GL_COLOR_ATTACHMENT12,
		GX_COLOR_ATTACHMENT13 = GL_COLOR_ATTACHMENT13,
		GX_COLOR_ATTACHMENT14 = GL_COLOR_ATTACHMENT14,
		GX_COLOR_ATTACHMENT15 = GL_COLOR_ATTACHMENT15,
		GX_COLOR_ATTACHMENT16 = GL_COLOR_ATTACHMENT16,
		GX_COLOR_ATTACHMENT17 = GL_COLOR_ATTACHMENT17,
		GX_COLOR_ATTACHMENT18 = GL_COLOR_ATTACHMENT18,
		GX_COLOR_ATTACHMENT19 = GL_COLOR_ATTACHMENT19,
		GX_COLOR_ATTACHMENT20 = GL_COLOR_ATTACHMENT20,
		GX_COLOR_ATTACHMENT21 = GL_COLOR_ATTACHMENT21,
		GX_COLOR_ATTACHMENT22 = GL_COLOR_ATTACHMENT22,
		GX_COLOR_ATTACHMENT23 = GL_COLOR_ATTACHMENT23,
		GX_COLOR_ATTACHMENT24 = GL_COLOR_ATTACHMENT24,
		GX_COLOR_ATTACHMENT25 = GL_COLOR_ATTACHMENT25,
		GX_COLOR_ATTACHMENT26 = GL_COLOR_ATTACHMENT26,
		GX_COLOR_ATTACHMENT27 = GL_COLOR_ATTACHMENT27,
		GX_COLOR_ATTACHMENT28 = GL_COLOR_ATTACHMENT28,
		GX_COLOR_ATTACHMENT29 = GL_COLOR_ATTACHMENT29,
		GX_COLOR_ATTACHMENT30 = GL_COLOR_ATTACHMENT30,
		GX_COLOR_ATTACHMENT31 = GL_COLOR_ATTACHMENT31,
		GX_DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
		GX_STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT,
		GX_DEPTH_STENCIL_ATTACHMENT = GL_DEPTH_STENCIL_ATTACHMENT
	};
	//TEXTURES
	enum GXTexture2DType {//TODO move this from here
		GX_NONE = 0,
		GX_DIFFUSE,
		GX_SPECULAR,
		GX_SPECULAR_MAP,
		GX_NORMAL,
		GX_HEIGHT,
		GX_AMBIENT
	};
	enum GXTexture2DColorType {
		GX_RGBA = GL_RGBA,
		GX_RGB = GL_RGB,
		GX_RED = GL_RED
	};

	enum GXTexture2DFilteringMethod {
		GX_NEAREST = GL_NEAREST,
		GX_LINEAR = GL_LINEAR
	};

	//SHADERS
	enum GXCompiledShader
	{
		DEFAULT_COLOR = 0,//ADD MORE TYPES THAT ARE PRECOMPILED FOR THE USER
		DEFAULT_LIGHT,
		DEFAULT_MODEL,
		DEFAULT_SKYDOME,
		DEFAULT_PLANE,
		DEFAULT_GBUFFER,
		DEFAULT_DEFERRED
	};
	enum GXShaderType {
		GX_VERTEX_SHADER = GL_VERTEX_SHADER,
		GX_FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		GX_COMPUTE_SHADER = GL_COMPUTE_SHADER,
		GX_GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
		GX_TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
		GX_TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER

	};
	enum GXFBOBindMethod {
		GX_FBO_READ = GL_READ_FRAMEBUFFER,
		GX_FBO_WRITE = GL_DRAW_FRAMEBUFFER,
		GX_FBO_RW = GL_FRAMEBUFFER
	};
#endif


}