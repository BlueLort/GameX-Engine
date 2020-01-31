#include "pch.h"
#include "NoiseGeneratorLayer.h"

namespace gx {
	void NoiseGeneratorLayer::init(int width, int height)
	{
		this->width = width;
		this->height = height;

	}

	void NoiseGeneratorLayer::destroy()
	{

	}

	void NoiseGeneratorLayer::start()
	{
	}

	void NoiseGeneratorLayer::end()
	{
	}

	int NoiseGeneratorLayer::onEvent(const gx::event::GXEventType& eventType)
	{
		return 0;
	}

	void NoiseGeneratorLayer::onUpdate(float deltaTime)
	{

	}

	void NoiseGeneratorLayer::onGUIRender()
	{


	}

}