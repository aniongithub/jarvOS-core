#include <jarvOS/jOS.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
    jOS_Lib libHdl;
    auto result = jOSLibInit(&libHdl);

    char errorName[256] = {0};
    size_t errorNameSizeBytes = 256;
    
    if (result != jOS_RESULT_OK)
    {
        jOSGetResultString(result, &errorName[0], &errorNameSizeBytes);
        printf("Could not initalize jarvOS, error was %s\n", errorName);
        return result;
    }
    
    printf("Initialized jarvOS lib, shutting down...\n");
    result = jOSLibShutdown(libHdl);
    if (result != jOS_RESULT_OK)
    {
        jOSGetResultString(result, &errorName[0], &errorNameSizeBytes);
        printf("Could not shutdown jarvOS, error was %s\n", errorName);
        return result;
    }
    
    printf("Shutdown jarvOS lib successfully\n");
    return result;
}