#pragma once

#include <QString>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <memory>

/**
 * @class Logger
 * @brief Thread-safe singleton logger with severity levels.
 *
 * Writes timestamped entries to both stderr and an optional log file.
 * All public methods are safe to call from any thread.
 */
class Logger
{
public:
    enum class Level { Debug, Info, Warning, Error };

    static Logger& instance();

    void setLevel(Level minimum);
    void setOutputFile(const QString& path);

    void log(Level level, const QString& module, const QString& message);

    void debug  (const QString& module, const QString& message);
    void info   (const QString& module, const QString& message);
    void warning(const QString& module, const QString& message);
    void error  (const QString& module, const QString& message);

private:
    Logger() = default;
    ~Logger();

    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;

    QString levelTag(Level level) const;

    QMutex                       m_mutex;
    Level                        m_minimum { Level::Debug };
    std::unique_ptr<QFile>       m_file;
    std::unique_ptr<QTextStream> m_stream;
};

/* -- Convenience macros -------------------------------------------- */

#define LOG_DEBUG(mod, msg)   Logger::instance().debug  (mod, msg)
#define LOG_INFO(mod, msg)    Logger::instance().info   (mod, msg)
#define LOG_WARN(mod, msg)    Logger::instance().warning(mod, msg)
#define LOG_ERROR(mod, msg)   Logger::instance().error  (mod, msg)
