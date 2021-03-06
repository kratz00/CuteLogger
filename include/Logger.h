/*
  Copyright (c) 2012 Boris Moiseev (cyberbobs at gmail dot com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1
  as published by the Free Software Foundation and appearing in the file
  LICENSE.LGPL included in the packaging of this file.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.
*/
#ifndef LOGGER_H
#define LOGGER_H

// Qt
#include <QString>
#include <QDebug>
#include <QDateTime>

// Local
#include "CuteLogger_global.h"
class AbstractAppender;


class Logger;
CUTELOGGERSHARED_EXPORT Logger* loggerInstance();
#define logger loggerInstance()


#define LOG_TRACE(...)       loggerInstance()->write(Logger::Trace,   __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__)
#define LOG_DEBUG(...)       loggerInstance()->write(Logger::Debug,   __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__)
#define LOG_INFO(...)        loggerInstance()->write(Logger::Info,    __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__)
#define LOG_WARNING(...)     loggerInstance()->write(Logger::Warning, __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__)
#define LOG_ERROR(...)       loggerInstance()->write(Logger::Error,   __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__)
#define LOG_FATAL(...)       loggerInstance()->write(Logger::Fatal,   __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__)

#define LOG_TRACE_TIME(...)  LoggerTimingHelper loggerTimingHelper(loggerInstance(), Logger::Trace, __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__)
#define LOG_DEBUG_TIME(...)  LoggerTimingHelper loggerTimingHelper(loggerInstance(), Logger::Debug, __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__)
#define LOG_INFO_TIME(...)   LoggerTimingHelper loggerTimingHelper(loggerInstance(), Logger::Info,  __FILE__, __LINE__, Q_FUNC_INFO, ##__VA_ARGS__)

#define LOG_ASSERT(cond) ((!(cond)) ? loggerInstance()->writeAssert(__FILE__, __LINE__, Q_FUNC_INFO, #cond) : qt_noop())


class LoggerPrivate;
class CUTELOGGERSHARED_EXPORT Logger
{
  Q_DISABLE_COPY(Logger)

  public:
    Logger();
    ~Logger();

    //! Describes the possible severity levels of the log records
    enum LogLevel
    {
      Trace,   //!< Trace level. Can be used for mostly unneeded records used for internal code tracing.
      Debug,   //!< Debug level. Useful for non-necessary records used for the debugging of the software.
      Info,    //!< Info level. Can be used for informational records, which may be interesting for not only developers.
      Warning, //!< Warning. May be used to log some non-fatal warnings detected by your application.
      Error,   //!< Error. May be used for a big problems making your application work wrong but not crashing.
      Fatal    //!< Fatal. Used for unrecoverable errors, crashes the application right after the log record is written.
    };

    static QString levelToString(LogLevel logLevel);
    static LogLevel levelFromString(const QString& s);

    static Logger* globalInstance();

    void registerAppender(AbstractAppender* appender);

    void write(const QDateTime& timeStamp, LogLevel logLevel, const char* file, int line, const char* function, const QString& message);
    void write(LogLevel logLevel, const char* file, int line, const char* function, const QString& message);
    void write(LogLevel logLevel, const char* file, int line, const char* function, const char* message, ...);
    QDebug write(LogLevel logLevel, const char* file, int line, const char* function);

    void writeAssert(const char* file, int line, const char* function, const char* condition);

  private:
    Q_DECLARE_PRIVATE(Logger)
    LoggerPrivate* d_ptr;
};


class CUTELOGGERSHARED_EXPORT LoggerTimingHelper
{
  Q_DISABLE_COPY(LoggerTimingHelper)

  public:
    inline explicit LoggerTimingHelper(Logger* l, Logger::LogLevel logLevel, const char* file, int line,
                                       const char* function, const QString& block = QString())
      : m_logger(l),
        m_logLevel(logLevel),
        m_file(file),
        m_line(line),
        m_function(function),
        m_block(block)
    {
      m_time.start();
    }

    ~LoggerTimingHelper();

  private:
    Logger* m_logger;
    QTime m_time;
    Logger::LogLevel m_logLevel;
    const char* m_file;
    int m_line;
    const char* m_function;
    QString m_block;
};


#endif // LOGGER_H
