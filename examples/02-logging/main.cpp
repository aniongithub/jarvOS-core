#include <jarvOS/jOS.h>

#include <stdio.h>
#include <cassert>

void onLog(jOS_LogLevel level, const char* function, size_t line, const char* file, const char* message)
{
    printf("%s\n", message);
}

int main(int argc, char *argv[])
{
    jOS_Lib libHdl;
    auto result = jOSLibInit(&libHdl);
    assert(result == jOS_RESULT_OK);

    jOS_LoggerHandle logger;
    jOS_LoggerParams loggerParams = {};
    loggerParams.logLevel = jOS_LOGLEVEL_DEBUG;
    loggerParams.onLog = onLog;
    result = jOSCreateLogger(libHdl, loggerParams, &logger);
    assert(result == jOS_RESULT_OK);

    result = jOSLog(jOS_LOGLEVEL_DEBUG, "", __LINE__, __FILE__, "Test log message from jarvOS client");
    assert(result == jOS_RESULT_OK);

    result = jOSDestroyLogger(libHdl, logger);
    assert(result == jOS_RESULT_OK);
    
    result = jOSLibShutdown(libHdl);
    assert(result == jOS_RESULT_OK);

    return result;
}