# YLogger - 优雅、高性能的异步日志封装（基于 spdlog）

YLogger 是一个基于 spdlog 库构建的 C++ 日志封装层。它旨在提供一个线程安全、高性能异步写入的日志框架，并支持多文件输出和编译时级别优化。


## 核心特性

- 异步日志 (Asynchronous): 将日志写入操作卸载到后台线程，确保应用程序的主线程不会因磁盘 I/O 而阻塞。

- 多文件/多 Sink 支持: 轻松创建和管理多个日志器，将不同类型的日志（如系统、业务、错误）写入不同的文件，并可选地输出到控制台。

- 编译时优化: 通过预定义宏，可以在编译阶段完全剔除 TRACE 和 DEBUG 级别的日志代码，实现零运行时开销。

- 每日文件轮转 (Daily Rotation): 自动按天切割日志文件，便于维护和管理。

- 统一宏接口: 使用 YLOGGER_XXX(logger_name, ...) 宏，通过指定名称即可将日志写入对应的文件。

## 快速开始

1. 依赖
本项目依赖于 spdlog 库。请确保您的项目中已正确配置和链接 spdlog。

2. 初始化配置 (main.cpp)
在使用任何日志宏之前，必须先调用 InitSystem 初始化异步线程池，并创建所需的文件日志器。

```C++
    // ---------------------------------------------
    // 1. 初始化系统 (只调用一次)
    // ---------------------------------------------
    if (!YLogger::Logger::Instance().InitSystem(YLogger::LogLevel::trace)) {
        qWarning() << "Fatal: Log system initialization failed.";
        return 1;
    }
    
    // ---------------------------------------------
    // 2. 创建不同的文件日志器
    // ---------------------------------------------
    // 创建一个名为 "SYSTEM" 的文件日志器
    YLogger::Logger::Instance().CreateFileLogger("APP_LOG", "logs/app.log");
    YLogger::Logger::Instance().SetLevel("APP_LOG", YLogger::LogLevel::debug);
    
    // 创建一个名为 "BUSINESS" 的文件日志器，并将其设置为新的默认日志器
    YLogger::Logger::Instance().CreateFileLogger("USER_LOG", "logs/user.log");
    YLogger::Logger::Instance().SetLevel("USER_LOG", YLogger::LogLevel::debug);
    
    // ---------------------------------------------
    // 3. 多线程写入测试
    // ---------------------------------------------
    auto thread_func = [](int id) {
        for (int i = 0; i < 20; ++i) {
            // 写入文件
            YLOGGER_TRACE("APP_LOG", "worker {:d} thread writing system log: {}", id, i);
            YLOGGER_DEBUG("USER_LOG", "worker {:d} thread writing system log: {}", id, i);
        }
    };
    
    std::thread t1(thread_func,1);
    std::thread t2(thread_func,2);
    
    t1.join();
    t2.join();
    
    // ---------------------------------------------
    // 4. 释放资源 (程序结束前调用)
    // ---------------------------------------------
    YLogger::Logger::Instance().Release();
```

---

## 编译时优化
默认情况下，YLOGGER_TRACE 和 YLOGGER_DEBUG 宏会进行编译时优化。

要启用或禁用这些级别的日志输出，请在项目的编译选项中（例如 CMake 或 qmake .pro 文件）定义 SPDLOG_ACTIVE_LEVEL 宏。

```C++
    级别设置	效果	.pro 文件配置示例
    SPDLOG_LEVEL_TRACE	所有级别都将被编译。	DEFINES += SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE
    SPDLOG_LEVEL_INFO	TRACE 和 DEBUG 级别的日志代码将被完全剔除。	DEFINES += SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO
    SPDLOG_LEVEL_OFF	所有日志宏都将被剔除 ((void)0)。	DEFINES += SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_OFF
```
======================================

Paragraph.

Second Level Heading in Alternate Style
---------------------------------------

Paragraph.

[1]: https://www.qt.io
