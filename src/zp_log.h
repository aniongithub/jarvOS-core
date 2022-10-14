#pragma once

#include <zeropoint/zp.h>
#include <plog/Log.h>
#include <memory>

class ZP_LogAppender : public plog::IAppender
{
    private:
        ZP_OnLogFunc _logCallback;
    public:
        ZP_LogAppender(ZP_OnLogFunc logCallback);
        virtual void write(const plog::Record& record);
};

struct ZP_LoggerHandle_t {
    private:
        std::unique_ptr<ZP_LogAppender> _appender;
    public:
        ZP_Result init(ZP_LoggerParams params);
};