#include "Logger.h"

#include <QDateTime>
#include <QDebug>

Logger& Logger::instance()
{
    static Logger inst;
    return inst;
}

Logger::~Logger() = default;

void Logger::setLevel(Level minimum)
{
    QMutexLocker lock(&m_mutex);
    m_minimum = minimum;
}

void Logger::setOutputFile(const QString& path)
{
    QMutexLocker lock(&m_mutex);

    m_file   = std::make_unique<QFile>(path);
    m_stream = nullptr;

    if (m_file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        m_stream = std::make_unique<QTextStream>(m_file.get());
}

void Logger::log(Level level, const QString& module, const QString& message)
{
    QMutexLocker lock(&m_mutex);

    if (level < m_minimum)
        return;

    const QString timestamp = QDateTime::currentDateTime()
                                  .toString("yyyy-MM-dd hh:mm:ss.zzz");

    const QString entry = QString("[%1] [%2] [%3] %4")
                              .arg(timestamp)
                              .arg(levelTag(level))
                              .arg(module)
                              .arg(message);

    qDebug().noquote() << entry;

    if (m_stream)
    {
        *m_stream << entry << "\n";
        m_stream->flush();
    }
}

void Logger::debug  (const QString& m, const QString& msg) { log(Level::Debug,   m, msg); }
void Logger::info   (const QString& m, const QString& msg) { log(Level::Info,    m, msg); }
void Logger::warning(const QString& m, const QString& msg) { log(Level::Warning, m, msg); }
void Logger::error  (const QString& m, const QString& msg) { log(Level::Error,   m, msg); }

QString Logger::levelTag(Level level) const
{
    switch (level)
    {
        case Level::Debug:   return "DBG";
        case Level::Info:    return "INF";
        case Level::Warning: return "WRN";
        case Level::Error:   return "ERR";
    }
    return "???";
}
