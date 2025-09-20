#include <pv_lib.h>

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h> 
#include <plog/Appenders/ColorConsoleAppender.h> 
#ifndef NDEBUG
#include "plog/Initializers/RollingFileInitializer.h"
#endif

#include <pv_log.h>

pv_Lib_t::pv_Lib_t() 
{
    _loggers = std::vector<pv_LoggerHandle>();
    plog::init(plog::Severity::verbose, &_dynamicAppender);

    // Add a default appender if built in debug
    #ifndef NDEBUG
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        _dynamicAppender.addAppender(&consoleAppender);
    #endif
}

pv_Lib_t::~pv_Lib_t()
{   
    // Cleanup
    for (auto iter = _loggers.begin(); iter < _loggers.end(); iter++)
        delete *iter;
}

pv_Result pvLibInit(pv_Lib* libHdl)
{
    if (!libHdl)
    {
        PV_LOGERROR("Invalid arguments, libHdl was null");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    *libHdl = new pv_Lib_t();
    PV_LOGDEBUG("Created library handle {} successfully", static_cast<void*>(*libHdl));
    
    return PV_RESULT_OK;
}

pv_Result pvLibShutdown(pv_Lib libHdl)
{
    if (!libHdl)
    {
        PV_LOGERROR("Invalid arguments, libHdl was null");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    PV_LOGDEBUG("Deleting libHdl {}", static_cast<void*>(libHdl));
    delete libHdl;

    return PV_RESULT_OK;
}