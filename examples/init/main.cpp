#include <polyverse/pv.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
    pv_Lib libHdl;
    auto result = pvLibInit(&libHdl);

    char errorName[256] = {0};
    size_t errorNameSizeBytes = 256;
    
    if (result != PV_RESULT_OK)
    {
        pvGetResultString(result, &errorName[0], &errorNameSizeBytes);
        printf("Could not initalize Polyverse, error was %s\n", errorName);
        return result;
    }
    
    printf("Initialized Polyverse lib, shutting down...\n");
    result = pvLibShutdown(libHdl);
    if (result != PV_RESULT_OK)
    {
        pvGetResultString(result, &errorName[0], &errorNameSizeBytes);
        printf("Could not shutdown Polyverse, error was %s\n", errorName);
        return result;
    }
    
    printf("Shutdown Polyverse lib successfully\n");
    return result;
}