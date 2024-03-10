//
// Created by Yulei on 2024/3/8.
//

#ifndef SIMREX_LOGGER_H
#define SIMREX_LOGGER_H

// STL dependencies
#include <memory>
#include <string>
#include <mutex>
#include <shared_mutex>

// spdlog dependencies
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

namespace SimREX::GEM {

    class LoggerManager {
    public:
        static LoggerManager *getInstance() {
            static LoggerManager instance;
            return &instance;
        }

        LoggerManager(const LoggerManager &) = delete;

        LoggerManager &operator=(const LoggerManager &) = delete;

        std::shared_ptr<spdlog::logger> createLogger(const std::string &logger_name) {
            std::shared_lock<std::shared_mutex> lock(_mutex);
            auto logger = spdlog::get(logger_name);
            if (!logger) {
                if (_logToFile) {
                    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(_logFileName, false);
                    logger = std::make_shared<spdlog::logger>(logger_name, file_sink);
                } else {
                    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
                    logger = std::make_shared<spdlog::logger>(logger_name, stdout_sink);
                }
                spdlog::register_logger(logger);
                logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%l] %v");
            }
            return logger;
        }

        void setLogToFile(bool logToFile) {
            std::unique_lock<std::shared_mutex> lock(_mutex);
            _logToFile = logToFile;
        }

        void setLogFileName(const std::string &logFileName) {
            std::unique_lock<std::shared_mutex> lock(_mutex);
            _logFileName = logFileName;
        }

    private:
        LoggerManager() = default;

        bool _logToFile = false;
        std::string _logFileName = "default.log";
        mutable std::shared_mutex _mutex;
    };

}

#endif //SIMREX_LOGGER_H
