#pragma once

#include <Config.h>

namespace gx{
	class GX_DLL GameXApp
	{
	public:
		GameXApp();

		virtual ~GameXApp();

		void Start();
	};
	GameXApp* CreateApp();
}
