#pragma once

#include <memory>

#include <plog/Log.h>
#include <fmt/format.h>

#include <jarvOS/jOS.h>

#define JOS_LOGFATAL(msg, ...) jOSLog(jOS_LOGLEVEL_FATAL, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())
#define JOS_LOGERROR(msg, ...) jOSLog(jOS_LOGLEVEL_ERROR, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())
#define JOS_LOGWARNING(msg, ...) jOSLog(jOS_LOGLEVEL_WARNING, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())
#define JOS_LOGINFO(msg, ...) jOSLog(jOS_LOGLEVEL_INFO, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())
#define JOS_LOGDEBUG(msg, ...) jOSLog(jOS_LOGLEVEL_DEBUG, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())
#define JOS_LOGVERBOSE(msg, ...) jOSLog(jOS_LOGLEVEL_VERBOSE, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())

class jOS_LogAppender : public plog::IAppender
{
    private:
        jOS_OnLogFunc _logCallback;
    public:
        jOS_LogAppender(jOS_OnLogFunc logCallback);
        virtual void write(const plog::Record& record);
};

struct jOS_LoggerHandle_t {
    private:
        std::unique_ptr<jOS_LogAppender> _appender;
    public:
        jOS_Result init(jOS_LoggerParams params);

        plog::IAppender* appender() { return _appender.get(); }
};