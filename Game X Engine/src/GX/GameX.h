#pragma once

#include "Config.h"

namespace gx{
	class GX_DLL GameX
	{
	public:
		GameX();

		virtual ~GameX();

		void Start();
	};
	GameX* CreateApp();
}
