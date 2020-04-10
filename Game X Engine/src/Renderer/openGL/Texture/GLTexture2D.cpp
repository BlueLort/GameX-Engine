#include "pch.h"
#include "GLTexture2D.h"


namespace gx {



	void GLTexture2D::init(const uint8_t* data, GXint32 width, GXint32 height, GXTexture2DColorType colorType, GXTexture2DType Type, GXTexture2DFilteringMethod method) {

		this->type = Type;
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, method);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, method);
		glTexImage2D(GL_TEXTURE_2D, 0, colorType, width, height, 0, colorType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

}