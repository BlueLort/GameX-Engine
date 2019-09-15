#pragma once
#include "Config.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


#define GXE_PRINT(...) gx::Logger::getEngineLogger()->trace(__VA_ARGS__)
#define GXE_INFO(...) gx::Logger::getEngineLogger()->info(__VA_ARGS__)
#define GXE_DEBUG(...) gx::Logger::getEngineLogger()->debug(__VA_ARGS__)
#define GXE_WARN(...) gx::Logger::getEngineLogger()->warn(__VA_ARGS__)
#define GXE_ERROR(...) gx::Logger::getEngineLogger()->error(__VA_ARGS__)
#define GXE_FERROR(...) gx::Logger::getEngineLogger()->critical(__VA_ARGS__);

#define GX_PRINT(...) gx::Logger::getGameLogger()->trace(__VA_ARGS__)
#define GX_INFO(...) gx::Logger::getGameLogger()->info(__VA_ARGS__)
#define GX_DEBUG(...) gx::Logger::getGameLogger()->debug(__VA_ARGS__)
#define GX_WARN(...) gx::Logger::getGameLogger()->warn(__VA_ARGS__)
#define GX_ERROR(...) gx::Logger::getGameLogger()->error(__VA_ARGS__)
#define GX_FERROR(...) gx::Logger::getGameLogger()->critical(__VA_ARGS__);

namespace gx {

	class GX_DLL Logger
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getEngineLogger() { return engineLogger; };
		inline static std::shared_ptr<spdlog::logger>& getGameLogger() { return gameLogger; };
	private:
		static std::shared_ptr<spdlog::logger> engineLogger;
		static std::shared_ptr<spdlog::logger> gameLogger;
	};
}

