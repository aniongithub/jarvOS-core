#pragma once

#include <zeropoint/zp.h>
#include <vector>

struct ZP_Lib_t {
    private:
        std::vector<ZP_LoggerHandle> _loggers;
    public:
        ZP_Lib_t();
        ~ZP_Lib_t();

        std::vector<ZP_LoggerHandle>& loggers() { return _loggers; }
};