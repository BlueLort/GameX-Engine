#pragma once
#include "Object/GXObject.h"
namespace gx {
	class GX_DLL GXGrid : public GXObject
	{
	public:
		inline GXGrid(const char* objName = "NONE") :GXObject(GX_GRID, objName) {}
		virtual void init(const char* fileName, const char* shaderPath = nullptr) override {}
		virtual void init(const char* shaderPath = nullptr) override;
		virtual void update(float deltaTime) override;
		virtual void destroy() override;

	};

}


