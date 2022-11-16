#include <jOS_log.h>
#include <jOS_lib.h>

#include <plog/Log.h>
#include <plog/Init.h>

#include <vector>
#include <algorithm>

jOS_LogAppender::jOS_LogAppender(jOS_OnLogFunc logCallback)
{
    _logCallback = logCallback;
}
void jOS_LogAppender::write(const plog::Record& record)
{
    if (_logCallback)
        _logCallback((jOS_LogLevel)record.getSeverity(),
            record.getFunc(),
            record.getLine(),
            record.getFile(),
            record.getMessage());
}

jOS_Result jOS_LoggerHandle_t::init(jOS_LoggerParams params)
{
    if (!params.onLog)
        return jOS_RESULT_INVALID_ARGUMENTS;
    
    _appender = std::unique_ptr<jOS_LogAppender>(new jOS_LogAppender(params.onLog));
    return jOS_RESULT_OK;
}

jOS_Result jOSCreateLogger(jOS_Lib libHdl, jOS_LoggerParams params, jOS_LoggerHandle* logger)
{
    auto hdl = new jOS_LoggerHandle_t();
    auto result =  hdl->init(params);
    if (result != jOS_RESULT_OK)
    {
        JOS_LOGERROR("Could not create logger, error was {}", result);
        delete hdl;
        return result;
    }
    
    libHdl->loggers().emplace_back(hdl);
    libHdl->dynamicAppender().addAppender(hdl->appender());
    *logger = hdl;
    JOS_LOGDEBUG("Successfully created logger {}!", static_cast<void*>(hdl));

    return jOS_RESULT_OK;
}

jOS_Result jOSLog(jOS_LogLevel level, const char* function, size_t line, const char* file, const char* message)
{
    plog::Record record((plog::Severity)level, function, line, file, nullptr, 0);
    record << message;
    *plog::get<PLOG_DEFAULT_INSTANCE_ID>() += record;
    return jOS_RESULT_OK;
}

jOS_Result jOSDestroyLogger(jOS_Lib libHdl, jOS_LoggerHandle logger)
{
    if (!logger)
        return jOS_RESULT_INVALID_ARGUMENTS;
    auto iter = std::find(libHdl->loggers().begin(), libHdl->loggers().end(), logger);
    if (iter == libHdl->loggers().end())
    {
        JOS_LOGERROR("Could not find logger with handle {}", static_cast<void*>(logger));
        return jOS_RESULT_INVALID_ARGUMENTS;
    }
    
    libHdl->dynamicAppender().removeAppender(logger->appender());
    libHdl->loggers().erase(iter);
    JOS_LOGDEBUG("Logger {} destroyed!", static_cast<void*>(logger));
    delete logger;

    return jOS_RESULT_OK;
}