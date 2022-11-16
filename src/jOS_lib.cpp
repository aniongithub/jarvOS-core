#include <jOS_lib.h>

#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/TxtFormatter.h> 
#include <plog/Appenders/ColorConsoleAppender.h> 
#ifndef NDEBUG
#include "plog/Initializers/RollingFileInitializer.h"
#endif

#include <jOS_log.h>

jOS_Lib_t::jOS_Lib_t() 
{
    _loggers = std::vector<jOS_LoggerHandle>();
    plog::init(plog::Severity::verbose, &_dynamicAppender);

    // Add a default appender if built in debug
    #ifndef NDEBUG
        static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
        _dynamicAppender.addAppender(&consoleAppender);
    #endif
}

jOS_Lib_t::~jOS_Lib_t()
{   
    // Cleanup
    for (auto iter = _loggers.begin(); iter < _loggers.end(); iter++)
        delete *iter;
}

jOS_Result jOSLibInit(jOS_Lib* libHdl)
{
    if (!libHdl)
    {
        JOS_LOGERROR("Invalid arguments, libHdl was null");
        return jOS_RESULT_INVALID_ARGUMENTS;
    }
    
    *libHdl = new jOS_Lib_t();
    JOS_LOGDEBUG("Created library handle {} successfully", static_cast<void*>(*libHdl));
    
    return jOS_RESULT_OK;
}

jOS_Result jOSLibShutdown(jOS_Lib libHdl)
{
    if (!libHdl)
    {
        JOS_LOGERROR("Invalid arguments, libHdl was null");
        return jOS_RESULT_INVALID_ARGUMENTS;
    }
    JOS_LOGDEBUG("Deleting libHdl {}", static_cast<void*>(libHdl));
    delete libHdl;

    return jOS_RESULT_OK;
}