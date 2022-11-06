#pragma once

#include <jarvOS/jOS.h>
#include <plog/Log.h>
#include <memory>

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
};