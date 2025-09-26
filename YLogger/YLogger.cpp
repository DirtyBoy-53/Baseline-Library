#include "YLogger.h"
#include <iostream>
namespace YLogger {

// 静态成员实现单例模式
Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

// 构造函数
Logger::Logger() {
    // 可以在这里进行一些预设置，例如：
    // spdlog::set_error_handler([](const std::string& msg) {
    //     std::cerr << "SPDLOG ERROR: " << msg << std::endl;
    // });
}

bool Logger::InitSystem(LogLevel level) {
    if (m_is_initialized) {
        return true;
    }

    try {
        // 1. 初始化线程池：8192 队列深度，1 个后台写入线程
        spdlog::init_thread_pool(8192, 1);

        // 2. 设置全局刷新策略：每 3 秒刷新一次，或遇到 ERROR/CRITICAL 立即刷新
        spdlog::flush_every(std::chrono::seconds(3));
        spdlog::flush_on(spdlog::level::err);

        // 3. 设置全局级别 (虽然没有默认 logger，但这个设置会影响后面创建的 logger)
        spdlog::set_level(level);

        m_is_initialized = true;
        return true;

    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log system initialization failed: " << ex.what() << std::endl;
        return false;
    }
}

bool Logger::CreateFileLogger(const std::string& name, const std::string& filename, const bool &is_print)
{
    if (m_loggers.count(name)) {
        return true; // 已存在
    }

    try {
        // 收集所有需要的 Sinks
        std::vector<spdlog::sink_ptr> sinks;

        // 创建每日文件切割 Sink (每天午夜 00:00 轮转)
        auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
            filename, 0, 0
            );
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%n] [Tid:%t] %v");
        sinks.emplace_back(file_sink);

        if (is_print) {
            // 创建控制台 Sink
            auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            // 单独设置与文件不同的格式
            console_sink->set_pattern("[%H:%M:%S.%e] [%^%l%$] [%n] %v");
            sinks.emplace_back(console_sink); // 添加到 sinks 列表中
        }

        // 创建异步日志器
        auto logger = std::make_shared<spdlog::async_logger>(
            name,
            sinks.begin(), sinks.end(),
            spdlog::thread_pool(),
            spdlog::async_overflow_policy::block
            );

        logger->set_level(spdlog::level::trace); // 默认设置最低级别

        m_loggers[name] = logger;
        // TODO: 做了本地map管理，暂时注册到spdlog系统中
        // 注册后 可使用 std::shared_ptr<spdlog::logger> my_logger = spdlog::get("Logger名称");
        // spdlog::register_logger(logger); // 注册到spdlog系统
        return true;

    } catch (const spdlog::spdlog_ex& ex) {
        m_loggers["DEFAULT"]->error("Failed to create logger '{}': {}", name, ex.what());
        return false;
    }
}

void Logger::Release() {
    if (!m_is_initialized) return;

    // 1. 清理所有日志器实例
    spdlog::drop_all();
    m_loggers.clear();

    // 2. 释放线程池资源 (关键步骤!)
    spdlog::shutdown();
    m_is_initialized = false;
    std::cout << "Log system released successfully." << std::endl;
}

std::shared_ptr<spdlog::logger> Logger::GetLogger(const std::string& name) {
    if (m_loggers.count(name)) {
        return m_loggers.at(name);
    }
    // 如果找不到指定的日志器，返回全局默认日志器
    return spdlog::default_logger();
}

void Logger::SetLevel(const std::string& logger_name, LogLevel level) {
    if (m_loggers.count(logger_name)) {
        m_loggers.at(logger_name)->set_level(level);
    }
}

} // namespace YLog
