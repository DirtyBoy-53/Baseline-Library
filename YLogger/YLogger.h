#ifndef YLOGGER_H
#define YLOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "Spdlog/sinks/stdout_color_sinks.h"



namespace YLogger{
    using LogLevel = spdlog::level::level_enum;
}
#define YLOGGER_TRACE(logger_name, ...) \
    YLogger::Logger::Instance().Log(logger_name, YLogger::LogLevel::trace, __VA_ARGS__)

#define YLOGGER_DEBUG(logger_name, ...) \
        YLogger::Logger::Instance().Log(logger_name, YLogger::LogLevel::debug, __VA_ARGS__)

#define YLOGGER_INFO(logger_name, ...) \
        YLogger::Logger::Instance().Log(logger_name, YLogger::LogLevel::info, __VA_ARGS__)

#define YLOGGER_WARN(logger_name, ...) \
        YLogger::Logger::Instance().Log(logger_name, YLogger::LogLevel::warn, __VA_ARGS__)

#define YLOGGER_ERROR(logger_name, ...) \
        YLogger::Logger::Instance().Log(logger_name, YLogger::LogLevel::err, __VA_ARGS__)

#define YLOGGER_CRITICAL(logger_name, ...) \
        YLogger::Logger::Instance().Log(logger_name, YLogger::LogLevel::critical, __VA_ARGS__)

namespace YLogger {

class Logger{
public:
    // 1. 单例访问点
    static Logger& Instance();

    // 2. 初始化核心系统（只需要调用一次）
    bool InitSystem(LogLevel level = LogLevel::trace);

    // 3. 创建并注册新的文件日志器（用于多文件需求）
    bool CreateFileLogger(const std::string& name,
                          const std::string& filename,
                          const bool &is_print = false);

    // 4. 核心日志函数（可变参数模板）
    template <typename... Args>
    void Log(
        const std::string& logger_name,
        LogLevel level,
        const spdlog::fmt_lib::format_string<Args...>& fmt,
        Args&&... args
        ) {
        auto logger = GetLogger(logger_name);
        if (logger) {
            logger->log(level, fmt, std::forward<Args>(args)...);
        }
    }

    // 5. 释放所有资源（在程序退出前调用）
    void Release();

    // 6. 运行时设置级别
    void SetLevel(const std::string& logger_name, LogLevel level);

    // 根据名称获取日志器
    std::shared_ptr<spdlog::logger> GetLogger(const std::string& name);
private:
    // 私有化构造/析构函数和拷贝操作，确保单例
    Logger();
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    std::map<std::string, std::shared_ptr<spdlog::logger>> m_loggers;
    bool m_is_initialized = false;
};

}

#endif
