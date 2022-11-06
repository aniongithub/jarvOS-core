#include <jarvOS/jOS.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
    jOS_Lib libHdl;
    auto result = jOSLibInit(&libHdl);
    if (result != jOS_RESULT_OK)
    {
        printf("Could not initalize jarvOS, error was %d\n", result);
        return result;
    }
    
    printf("Initialized jarvOS lib, shutting down...\n");
    result = jOSLibShutdown(libHdl);
    if (result != jOS_RESULT_OK)
        printf("Could not shutdown jarvOS, error was %d\n", result);
    
    printf("Shutdown jarvOS lib successfully\n");
    return result;
}