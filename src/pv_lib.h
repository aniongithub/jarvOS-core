#pragma once

#include <vector>

#include <polyverse/pv.h>

#include <plog/Log.h> 
#include <plog/Appenders/DynamicAppender.h>
 
struct pv_Lib_t {
    private:
        plog::DynamicAppender _dynamicAppender;
        std::vector<pv_LoggerHandle> _loggers;
    public:
        pv_Lib_t();
        ~pv_Lib_t();

        std::vector<pv_LoggerHandle>& loggers() { return _loggers; }
        plog::DynamicAppender& dynamicAppender() { return _dynamicAppender; }
};