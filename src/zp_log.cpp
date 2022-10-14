#include <zp_log.h>
#include <zp_lib.h>

#include <plog/Log.h>
#include <plog/Init.h>

#include <vector>
#include <algorithm>

ZP_LogAppender::ZP_LogAppender(ZP_OnLogFunc logCallback)
{
    _logCallback = logCallback;
}
void ZP_LogAppender::write(const plog::Record& record)
{
    if (_logCallback)
        _logCallback((ZP_LogLevel)record.getSeverity(),
            record.getFunc(),
            record.getLine(),
            record.getFile(),
            record.getMessage());
}

ZP_Result ZP_LoggerHandle_t::init(ZP_LoggerParams params)
{
    if (!params.onLog)
        return ZP_RESULT_INVALID_ARGUMENTS;
    
    _appender = std::unique_ptr<ZP_LogAppender>(new ZP_LogAppender(params.onLog));
    plog::init((plog::Severity)params.logLevel, _appender.get());

    return ZP_RESULT_OK;
}

ZP_Result zpCreateLogger(ZP_Lib libHdl, ZP_LoggerParams params, ZP_LoggerHandle* logger)
{
    auto hdl = new ZP_LoggerHandle_t();
    auto result =  hdl->init(params);
    if (result != ZP_RESULT_OK)
    {
        delete hdl;
        return result;
    }
    
    libHdl->loggers().emplace_back(hdl);
    *logger = hdl;

    return ZP_RESULT_OK;
}

ZP_Result zpLog(ZP_LogLevel level, const char* function, size_t line, const char* file, const char* message)
{
    plog::Record record((plog::Severity)level, function, line, file, nullptr, 0);
    record << message;
    *plog::get<PLOG_DEFAULT_INSTANCE_ID>() += record;
    return ZP_RESULT_OK;
}

ZP_Result zpDestroyLogger(ZP_Lib libHdl, ZP_LoggerHandle logger)
{
    if (!logger)
        return ZP_RESULT_INVALID_ARGUMENTS;
    auto iter = std::find(libHdl->loggers().begin(), libHdl->loggers().end(), logger);
    if (iter == libHdl->loggers().end())
        return ZP_RESULT_INVALID_ARGUMENTS;
    libHdl->loggers().erase(iter);
    delete logger;

    return ZP_RESULT_OK;
}