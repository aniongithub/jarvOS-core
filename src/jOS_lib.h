#pragma once

#include <jarvOS/jOS.h>
#include <vector>

struct jOS_Lib_t {
    private:
        std::vector<jOS_LoggerHandle> _loggers;
    public:
        jOS_Lib_t();
        ~jOS_Lib_t();

        std::vector<jOS_LoggerHandle>& loggers() { return _loggers; }
};