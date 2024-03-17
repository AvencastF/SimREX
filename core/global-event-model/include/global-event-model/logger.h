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
#include <map>

// spdlog dependencies
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

namespace SimREX::GEM
{
    class LoggerManager {
    public:
        static LoggerManager* getInstance() {
            static LoggerManager instance;
            return &instance;
        }

        LoggerManager(const LoggerManager&) = delete;

        LoggerManager& operator=(const LoggerManager&) = delete;

        std::map<std::string, std::shared_ptr<spdlog::logger>> loggers;

        std::shared_ptr<spdlog::logger> createLogger(const std::string& logger_name) {
            std::shared_lock lock(_mutex);
            if (const auto it = loggers.find(logger_name); it != loggers.end()) {
                return it->second;
            }
            lock.unlock();

            std::shared_ptr<spdlog::logger> logger;
            if (_logToFile) {
                auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(_logFileName, true);
                logger = std::make_shared<spdlog::logger>(logger_name, file_sink);
            } else {
                auto stdout_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
                logger = std::make_shared<spdlog::logger>(logger_name, stdout_sink);
            }

            std::unique_lock<std::shared_mutex> write_lock(_mutex);
            loggers[logger_name] = logger;
            write_lock.unlock();

            logger->set_level(_logLevel);
            spdlog::register_logger(logger);
            logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%n] [%l] %v");

            return logger;
        }

        void setLogToFile(bool logToFile) {
            std::unique_lock<std::shared_mutex> lock(_mutex);
            _logToFile = logToFile;
        }

        void setLogFileName(const std::string& logFileName) {
            std::unique_lock<std::shared_mutex> lock(_mutex);
            _logFileName = logFileName;
        }

        [[nodiscard]] spdlog::level::level_enum get_log_level() const { return _logLevel; }

        void set_log_level(int log_level) {
            _logLevel = static_cast<spdlog::level::level_enum>(log_level);
        }

        //! Ensure all logging operations are finished
        void flush() {
            std::unique_lock<std::shared_mutex> lock(_mutex);
            for (const auto& [name, logger] : loggers) {
                logger->flush();
            }

            spdlog::shutdown();
        }

    private:
        LoggerManager() = default;

        spdlog::level::level_enum _logLevel = spdlog::level::info;

        bool _logToFile = false;
        std::string _logFileName = "default.log";
        mutable std::shared_mutex _mutex;
    };
}

#endif //SIMREX_LOGGER_H
