#include <zeropoint/zp.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
    ZP_Lib libHdl;
    auto result = zpLibInit(&libHdl);
    if (result != ZP_RESULT_OK)
    {
        printf("Could not initalize zeropoint, error was %d\n", result);
        return result;
    }
    
    printf("Initialized zeropoint lib, shutting down...\n");
    result = zpLibShutdown(libHdl);
    if (result != ZP_RESULT_OK)
        printf("Could not shutdown zeropoint, error was %d\n", result);
    
    printf("Shutdown zeropoint lib successfully\n");
    return result;
}