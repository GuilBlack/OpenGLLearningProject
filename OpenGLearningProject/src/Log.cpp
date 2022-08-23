#include "pch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::s_EngineLogger;

void Log::Init()
{
	spdlog::set_pattern("%^[%T] [thread %t] %n: %v%$");
	s_EngineLogger = spdlog::stdout_color_mt("EngineLog");
	s_EngineLogger->set_level(spdlog::level::trace);
}