#include <zp_log.h>

#include <plog/Log.h>
#include <vector>

#ifndef NDEBUG
#include <plog/Initializers/RollingFileInitializer.h>
#endif

std::vector<ZP_LoggerHandle> loggers;

ZP_Result ZP_LoggerHandle_t::init()
{
    return ZP_RESULT_NOT_IMPLEMENTED;
}

ZP_Result zpCreateLogger(ZP_LoggerParams params, ZP_LoggerHandle* logger)
{
    auto hdl = new ZP_LoggerHandle_t();
    auto result =  hdl->init();
    if (result != ZP_RESULT_OK)
        delete hdl;
        return result;
    
    loggers.emplace_back(hdl);
    *logger = hdl;
    
    return ZP_RESULT_OK;
}

ZP_Result zpDestroyLogger(ZP_LoggerHandle logger)
{
    if (!logger)
        return ZP_RESULT_INVALID_ARGUMENTS;
    auto iter = std::find(loggers.begin(), loggers.end(), logger);
    if (iter == loggers.end())
        return ZP_RESULT_INVALID_ARGUMENTS;
    loggers.erase(iter);
    delete logger;

    return ZP_RESULT_OK;
}