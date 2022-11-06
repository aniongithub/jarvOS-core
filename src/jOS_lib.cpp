#include <jOS_lib.h>

#include <plog/Log.h>
#ifndef NDEBUG
#include "plog/Initializers/RollingFileInitializer.h"
#endif

#include <jOS_log.h>

jOS_Lib_t::jOS_Lib_t() 
{
    _loggers = std::vector<jOS_LoggerHandle>();
}

jOS_Lib_t::~jOS_Lib_t()
{   
    // Cleanup
    for (auto iter = _loggers.begin(); iter < _loggers.end(); iter++)
        delete *iter;
}

jOS_Result jOSLibInit(jOS_Lib* libHdl)
{
    // If debug, add a text logger by default
    #ifndef NDEBUG
    plog::init(plog::debug, "jOS.log.txt");
    #endif

    if (!libHdl)
    {
        PLOGD << "Invalid arguments, libHdl was null";
        return jOS_RESULT_INVALID_ARGUMENTS;
    }
    
    *libHdl = new jOS_Lib_t();
    PLOGD << "Created library handle successfully";
    
    return jOS_RESULT_OK;
}

jOS_Result jOSLibShutdown(jOS_Lib libHdl)
{
    if (!libHdl)
    {
        PLOGD << "Invalid arguments, libHdl was null";
        return jOS_RESULT_INVALID_ARGUMENTS;
    }
    PLOGD << "Deleting libHdl";
    delete libHdl;

    return jOS_RESULT_OK;
}