#pragma once;
#include "easy-log.h"

#define TRACE() log_stream({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::trace, "")
#define DBG() log_stream({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::debug, "")
#define INF() log_stream({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::info, "")
#define WRN() log_stream({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::warn, "")
#define ERR() log_stream({__FILE__, __LINE__, __FUNCTION__}, spdlog::level::err, "")