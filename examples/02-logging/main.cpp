#include <zeropoint/zp.h>

#include <stdio.h>
#include <cassert>

void onLog(ZP_LogLevel level, const char* function, size_t line, const char* file, const char* message)
{
    printf("%s\n", message);
}

int main(int argc, char *argv[])
{
    ZP_Lib libHdl;
    auto result = zpLibInit(&libHdl);
    assert(result == ZP_RESULT_OK);

    ZP_LoggerHandle logger;
    ZP_LoggerParams loggerParams = {};
    loggerParams.logLevel = ZP_LOGLEVEL_DEBUG;
    loggerParams.onLog = onLog;
    result = zpCreateLogger(libHdl, loggerParams, &logger);
    assert(result == ZP_RESULT_OK);

    result = zpLog(ZP_LOGLEVEL_DEBUG, "", __LINE__, __FILE__, "Test log message from zeropoint client");
    assert(result == ZP_RESULT_OK);

    result = zpDestroyLogger(libHdl, logger);
    assert(result == ZP_RESULT_OK);
    
    result = zpLibShutdown(libHdl);
    assert(result == ZP_RESULT_OK);

    return result;
}