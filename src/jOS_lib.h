#pragma once

#include <vector>

#include <jarvOS/jOS.h>

#include <plog/Log.h> 
#include <plog/Appenders/DynamicAppender.h>
 
struct jOS_Lib_t {
    private:
        plog::DynamicAppender _dynamicAppender;
        std::vector<jOS_LoggerHandle> _loggers;
    public:
        jOS_Lib_t();
        ~jOS_Lib_t();

        std::vector<jOS_LoggerHandle>& loggers() { return _loggers; }
        plog::DynamicAppender& dynamicAppender() { return _dynamicAppender; }
};