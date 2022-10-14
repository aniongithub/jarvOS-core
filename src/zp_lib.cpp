#include <zp_lib.h>

#include <plog/Log.h>
#ifndef NDEBUG
#include "plog/Initializers/RollingFileInitializer.h"
#endif

#include <zp_log.h>

ZP_Lib_t::ZP_Lib_t() 
{
    _loggers = std::vector<ZP_LoggerHandle>();
}

ZP_Lib_t::~ZP_Lib_t()
{   
    // Cleanup
    for (auto iter = _loggers.begin(); iter < _loggers.end(); iter++)
        delete *iter;
}

ZP_Result zpLibInit(ZP_Lib* libHdl)
{
    // If debug, add a text logger by default
    #ifndef NDEBUG
    plog::init(plog::debug, "zp.log.txt");
    #endif

    if (!libHdl)
    {
        PLOGD << "Invalid arguments, libHdl was null";
        return ZP_RESULT_INVALID_ARGUMENTS;
    }
    
    *libHdl = new ZP_Lib_t();
    PLOGD << "Created library handle successfully";
    
    return ZP_RESULT_OK;
}

ZP_Result zpLibShutdown(ZP_Lib libHdl)
{
    if (!libHdl)
    {
        PLOGD << "Invalid arguments, libHdl was null";
        return ZP_RESULT_INVALID_ARGUMENTS;
    }
    PLOGD << "Deleting libHdl";
    delete libHdl;

    return ZP_RESULT_OK;
}