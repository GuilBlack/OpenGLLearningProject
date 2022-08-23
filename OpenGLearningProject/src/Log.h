#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Log
{
public:
	static void Init();
	inline static std::shared_ptr<spdlog::logger> GetEngineLogger() { return s_EngineLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_EngineLogger;
};

#ifdef DEBUG

#define ENGINE_ERROR(...) Log::GetEngineLogger()->error(__VA_ARGS__)
#define ENGINE_WARN(...)  Log::GetEngineLogger()->warn(__VA_ARGS__)
#define ENGINE_INFO(...)  Log::GetEngineLogger()->info(__VA_ARGS__)
#define ENGINE_TRACE(...) Log::GetEngineLogger()->trace(__VA_ARGS__)

#else

#define ENGINE_ERROR(...)
#define ENGINE_WARN(...)
#define ENGINE_INFO(...)
#define ENGINE_TRACE(...)

#endif // DEBUG

