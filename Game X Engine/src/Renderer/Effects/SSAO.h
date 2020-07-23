#pragma once
#include "pch.h"
#include "Config.h"
#include "Renderer/Buffers/GXFrameBufferObject.h"
#include "Renderer/Enums.h"
#include "Maths/GXMaths.h"
#include "Renderer/Shader/GXShaderManager.h"

namespace gx {
	//TODO RENDERING SIZE AND ADAPT learnopengl.com METHOD TO GX Engine 
	class GX_DLL GXSSAO
	{
	public:
		inline void init(GXint32 width, GXint32 height) {
			SSAOBlurBuffer.reset(new GXFrameBuffer());
			SSAOBlurBuffer->init(width, height);
			SSAOBlurBuffer->addTextureAttachment(GX_COLOR_TEXTURE);
			SSAOBuffer.reset(new GXFrameBuffer());
			SSAOBuffer->init(width, height);
			SSAOBuffer->addTextureAttachment(GX_COLOR_TEXTURE);


			/*SSAOShader = GXShaderManager::getShader(GXCompiledShader::DEFAULT_SSAO);
			SSAOShader->use();
			SSAOShader->setInt("gPosition", 0);
			SSAOShader->setInt("gNormal", 1);
			SSAOShader->setInt("texNoise", 2);

			SSAOBlurShader = GXShaderManager::getShader(GXCompiledShader::DEFAULT_SSAO_BLUR);
			SSAOBlurShader->use();
			SSAOBlurShader->setInt("ssaoInput", 0);*/

			// Setup SSAO Stuff , Sample Kernel & Noise 
			// Technique as explained in learnopengl.com
			std::uniform_real_distribution<GXFloat> randomFloats(0.0, 1.0);
			std::default_random_engine generator;
			std::vector<GXVec3> SSAOKernel;
			for (GXuint32 i = 0; i < 64; ++i)
			{
				GXVec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
				sample = GXMaths::normalize(sample);
				sample *= randomFloats(generator);
				GXFloat scale = GXFloat(i) / 64.0;

				// scale samples s.t. they're more aligned to center of kernel
				scale = GXMaths::lerp(0.1f, 1.0f, scale * scale);
				sample *= scale;
				SSAOKernel.push_back(sample);
			}
			std::vector<GXVec3> SSAONoise;
			for (GXuint32 i = 0; i < 16; i++)
			{
				GXVec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
				SSAONoise.push_back(noise);
			}
			GXTexture2D tex;
			tex.init(&SSAONoise[0], 4, 4, GXTexture2DColorType::GX_RGB32F, GXTexture2DColorType::GX_RGB, GXTexture2DType::GX_DIFFUSE, GXTexture2DFilteringMethod::GX_NEAREST, GXEnumType::GX_FLOAT);

		}
	private:
		std::unique_ptr<GXFrameBuffer> SSAOBuffer;
		std::unique_ptr<GXFrameBuffer> SSAOBlurBuffer;
		GXShader* SSAOShader = nullptr;
		GXShader* SSAOBlurShader = nullptr;
	};
}
