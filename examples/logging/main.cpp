#include <polyverse/pv.h>

#include <stdio.h>
#include <cassert>

#include <fmt/format.h>

void onLog(pv_LogLevel level, const char* function, size_t line, const char* file, const char* message)
{
    printf("%s\n", message);
}

int main(int argc, char *argv[])
{
    pv_Lib libHdl;
    auto result = pvLibInit(&libHdl);
    assert(result == PV_RESULT_OK);

    pv_LoggerHandle logger;
    pv_LoggerParams loggerParams = {};
    loggerParams.logLevel = PV_LOGLEVEL_DEBUG;
    loggerParams.onLog = onLog;
    result = pvCreateLogger(libHdl, loggerParams, &logger);
    assert(result == PV_RESULT_OK);

    // result = pvLog(pv_LOGLEVEL_DEBUG, "", __LINE__, __FILE__, "Test log message from Polyverse client");
    result = pvLog(pv_LogLevel::PV_LOGLEVEL_DEBUG, __PRETTY_FUNCTION__, __LINE__, __FILE__, fmt::format("Test log message from Polyverse client").c_str());
    assert(result == PV_RESULT_OK);

    result = pvDestroyLogger(libHdl, logger);
    assert(result == PV_RESULT_OK);
    
    result = pvLibShutdown(libHdl);
    assert(result == PV_RESULT_OK);

    return result;
}