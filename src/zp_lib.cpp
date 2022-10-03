#include <zp_lib.h>

#include <plog/Log.h>
#ifndef NDEBUG
#include "plog/Initializers/RollingFileInitializer.h"
#endif

ZP_Lib_t::ZP_Lib_t() 
{
    // Nothing to do yet   
}

ZP_Lib_t::~ZP_Lib_t()
{
    // Nothing to do yet
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
    PLOGV << "Created library handle successfully";
    
    return ZP_RESULT_OK;
}

ZP_Result zpLibShutdown(ZP_Lib libHdl)
{
    if (!libHdl)
    {
        PLOGD << "Invalid arguments, libHdl was null";
        return ZP_RESULT_INVALID_ARGUMENTS;
    }
    delete libHdl;
    PLOGV << "Deleted libHdl successfully";

    return ZP_RESULT_OK;
}