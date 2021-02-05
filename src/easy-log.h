//  static function check dir 
// Check only one layer
#ifndef __EASY_LOG_H__
#define __EASY_LOG_H__

#pragma once

#ifdef WIN32
#define NOMINMAX
#undef min
#undef max
#endif

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/logger.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
//#include <spdlog/details/pattern_formatter.h>
#include <fmt/printf.h>

#ifdef WIN32  
#include <io.h>
#include <direct.h>  
#endif  
#ifdef linux   
#include <unistd.h> 
#include <sys/types.h>  
#include <sys/stat.h> 
#endif  

#include <sstream>
#include <memory>
#include <future>

#include <filesystem>

static  std::atomic_int s_initState;

// spdlog wrapper
class SpdLogWrapper
{

public:
	SpdLogWrapper()
	{
	}

	inline ~SpdLogWrapper()
	{
	}

	static SpdLogWrapper& inst()
	{
		static SpdLogWrapper log;
		return log;
	}

	bool init(const std::string& logDir, const std::string& logFileName)
	{
		// get full path
		std::string fullPath;
		if (logFileName.empty())
		{
			// if logFileName is null then logDir is the fullpath
			fullPath = logDir;
			if (fullPath.empty())
			{
				// LOG_ERROR("log path is null.");
				return false;
			}
			// get real directory of path
			auto pos = fullPath.find_last_of("/\\");
			if (pos != fullPath.npos)
			{
				auto pathDir = fullPath.substr(0, pos);
				if (pathDir.empty())
					return false;

				// check directory is exist
				if (!check_dir(pathDir.c_str()))
					return false;
			}
		}
		else
		{
			if (!check_dir(logDir.c_str()))
				return false;

			fullPath = logDir + "/" + logFileName;
		}
		if (fullPath.empty())
			return false;

		{
			std::error_code ec;
			fullPath = std::filesystem::absolute(std::filesystem::u8path(fullPath), ec).string();
		}

		// check is inited
		if (s_initState)
			return true;

		spdlog::init_thread_pool(32768, std::thread::hardware_concurrency());
		spdlog::set_level(spdlog::level::debug);
		spdlog::flush_on(spdlog::level::info);
		spdlog::set_pattern("%Y-%m-%d %T.%e:%s(%#):[%l] %v");
		auto logger = spdlog::daily_logger_mt("daily_logger", fullPath, 0, 2, true);
		spdlog::set_default_logger(logger);

#ifdef WIN32
#ifdef _DEBUG
		// Visual Studio console log
		auto mssink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
		if (!mssink)
			return false;
		mssink->set_pattern("%Y-%m-%d %T.%e:%s(%#):[%l] %v");
		logger->sinks().push_back(mssink);
#endif // _DEBUG
#endif

		s_initState = true;
		return true;
	}

	void setLogLevel(int level)
	{
		spdlog::set_level(spdlog::level::level_enum(level));
		spdlog::flush_on(spdlog::level::level_enum(level));
	}

	void destory()
	{
		spdlog::shutdown();
	}

	template <typename... Args>
	void out(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, const char *fmt, const Args &... args)
	{
		auto content = fmt::sprintf(fmt, args...);
		content.erase(content.find_last_not_of("\n\r\t") + 1);
		spdlog::log(loc, lvl, content.c_str());
	}

	template <typename... Args>
	void printf(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, const char *fmt, const Args &... args)
	{
		auto content = fmt::sprintf(fmt, args...);
		content.erase(content.find_last_not_of("\n\r\t") + 1);
		spdlog::log(loc, lvl, content.c_str());
	}

private:
	static bool check_dir(const char* dir)
	{
		std::error_code ec;
		std::filesystem::path p(std::filesystem::u8path(dir));
		std::filesystem::create_directories(p, ec);
		return !ec;
#ifdef WIN32
		if (_access(dir, 0) == -1)
#else
		if (access(dir, 0) != 0)
#endif
		{
#ifdef WIN32  
			int flag = _mkdir(dir);
#else
			int flag = mkdir(dir, 0777);
#endif  
			return (flag == 0);
		}
		return true;
	};

private:
	std::shared_ptr<spdlog::logger> logger_;
};

struct log_stream : public std::ostringstream
{
public:
	log_stream(const spdlog::source_loc& _loc, spdlog::level::level_enum _lvl, const std::string& _prefix)
		: loc(_loc)
		, lvl(_lvl)
		, prefix(_prefix)
	{
	}

	~log_stream()
	{
		flush();
	}

	void flush()
	{
		SpdLogWrapper::inst().out(loc, lvl, (prefix + str()).c_str());
	}

private:
	spdlog::source_loc loc;
	spdlog::level::level_enum lvl;
	std::string prefix;
};

#endif // __EASY_LOG_H__