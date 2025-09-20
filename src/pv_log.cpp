#include <pv_log.h>
#include <pv_lib.h>

#include <plog/Log.h>
#include <plog/Init.h>

#include <vector>
#include <algorithm>

pv_LogAppender::pv_LogAppender(pv_OnLogFunc logCallback)
{
    _logCallback = logCallback;
}
void pv_LogAppender::write(const plog::Record& record)
{
    if (_logCallback)
        _logCallback((pv_LogLevel)record.getSeverity(),
            record.getFunc(),
            record.getLine(),
            record.getFile(),
            record.getMessage());
}

pv_Result pv_LoggerHandle_t::init(pv_LoggerParams params)
{
    if (!params.onLog)
        return PV_RESULT_INVALID_ARGUMENTS;
    
    _appender = std::unique_ptr<pv_LogAppender>(new pv_LogAppender(params.onLog));
    return PV_RESULT_OK;
}

pv_Result pvCreateLogger(pv_Lib libHdl, pv_LoggerParams params, pv_LoggerHandle* logger)
{
    auto hdl = new pv_LoggerHandle_t();
    auto result =  hdl->init(params);
    if (result != PV_RESULT_OK)
    {
        PV_LOGERROR("Could not create logger, error was {}", result);
        delete hdl;
        return result;
    }
    
    libHdl->loggers().emplace_back(hdl);
    libHdl->dynamicAppender().addAppender(hdl->appender());
    *logger = hdl;
    PV_LOGDEBUG("Successfully created logger {}!", static_cast<void*>(hdl));

    return PV_RESULT_OK;
}

pv_Result pvLog(pv_LogLevel level, const char* function, size_t line, const char* file, const char* message)
{
    plog::Record record((plog::Severity)level, function, line, file, nullptr, 0);
    record << message;
    *plog::get<PLOG_DEFAULT_INSTANCE_ID>() += record;
    return PV_RESULT_OK;
}

pv_Result pvDestroyLogger(pv_Lib libHdl, pv_LoggerHandle logger)
{
    if (!logger)
        return PV_RESULT_INVALID_ARGUMENTS;
    auto iter = std::find(libHdl->loggers().begin(), libHdl->loggers().end(), logger);
    if (iter == libHdl->loggers().end())
    {
        PV_LOGERROR("Could not find logger with handle {}", static_cast<void*>(logger));
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    libHdl->dynamicAppender().removeAppender(logger->appender());
    libHdl->loggers().erase(iter);
    PV_LOGDEBUG("Logger {} destroyed!", static_cast<void*>(logger));
    delete logger;

    return PV_RESULT_OK;
}