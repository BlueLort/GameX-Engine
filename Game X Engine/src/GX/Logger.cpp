#include <pch.h>
#include "Logger.h"


namespace gx {
	std::shared_ptr<spdlog::logger> Logger::engineLogger;
	std::shared_ptr<spdlog::logger> Logger::gameLogger;

	void Logger::init() {
		spdlog::set_pattern("%^[%T] |%n| %v%$");
		engineLogger = spdlog::stdout_color_mt("GX Engine");
		gameLogger = spdlog::stdout_color_mt("X");
		engineLogger->set_level(spdlog::level::trace);
		gameLogger->set_level(spdlog::level::trace);
		GXE_INFO("Logger initialized successfully ...");
	}


}

