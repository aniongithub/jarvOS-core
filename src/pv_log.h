#pragma once

#include <memory>

#include <plog/Log.h>
#include <fmt/format.h>

#include <polyverse/pv.h>

#define PV_LOGFATAL(msg, ...) pvLog(PV_LOGLEVEL_FATAL, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())
#define PV_LOGERROR(msg, ...) pvLog(PV_LOGLEVEL_ERROR, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())
#define PV_LOGWARNING(msg, ...) pvLog(PV_LOGLEVEL_WARNING, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())
#define PV_LOGINFO(msg, ...) pvLog(PV_LOGLEVEL_INFO, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())
#define PV_LOGDEBUG(msg, ...) pvLog(PV_LOGLEVEL_DEBUG, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())
#define PV_LOGVERBOSE(msg, ...) pvLog(PV_LOGLEVEL_VERBOSE, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format(msg,##__VA_ARGS__).c_str())

class pv_LogAppender : public plog::IAppender
{
    private:
        pv_OnLogFunc _logCallback;
    public:
        pv_LogAppender(pv_OnLogFunc logCallback);
        virtual void write(const plog::Record& record);
};

struct pv_LoggerHandle_t {
    private:
        std::unique_ptr<pv_LogAppender> _appender;
    public:
        pv_Result init(pv_LoggerParams params);

        plog::IAppender* appender() { return _appender.get(); }
};