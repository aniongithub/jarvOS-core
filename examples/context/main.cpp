#include <polyverse/pv.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void onLog(pv_LogLevel level, const char* function, size_t line, const char* file, const char* message)
{
    // Only show important messages, not debug output
    if (level <= PV_LOGLEVEL_INFO) {
        printf("[INFO] %s\n", message);
    }
}

int main(int argc, char *argv[])
{
    printf("=== Polyverse Context API Example ===\n");
    printf("This example demonstrates how to use contexts to store and retrieve data.\n\n");

    // Initialize the Polyverse library
    pv_Lib libHdl;
    pv_Result result = pvLibInit(&libHdl);
    if (result != PV_RESULT_OK) {
        printf("Failed to initialize Polyverse library\n");
        return -1;
    }

    // Setup minimal logging (only show warnings and errors)
    pv_LoggerHandle logger = nullptr;
    pv_LoggerParams loggerParams = {};
    loggerParams.logLevel = PV_LOGLEVEL_WARNING;
    loggerParams.onLog = onLog;
    result = pvCreateLogger(libHdl, loggerParams, &logger);

    // Create a context - think of it as a key-value store
    pv_Context ctx;
    result = pvCreateContext(&ctx);
    if (result != PV_RESULT_OK) {
        printf("Failed to create context\n");
        return -1;
    }
    printf("✓ Created a new context\n");

    // Example 1: Store some configuration data
    printf("\n--- Storing Configuration Data ---\n");
    
    const char* appName = "MyApp";
    const char* version = "1.0.0";
    int maxConnections = 100;
    double timeout = 30.5;

    // Store string data
    pvSetContextData(ctx, (char*)"app_name", strlen("app_name"), 
                     (void*)appName, strlen(appName));
    printf("Stored app name: %s\n", appName);

    pvSetContextData(ctx, (char*)"version", strlen("version"), 
                     (void*)version, strlen(version));
    printf("Stored version: %s\n", version);

    // Store numeric data
    pvSetContextData(ctx, (char*)"max_connections", strlen("max_connections"), 
                     &maxConnections, sizeof(maxConnections));
    printf("Stored max connections: %d\n", maxConnections);

    pvSetContextData(ctx, (char*)"timeout", strlen("timeout"), 
                     &timeout, sizeof(timeout));
    printf("Stored timeout: %.1f seconds\n", timeout);

    // Example 2: Enumerate keys and safely retrieve data
    printf("\n--- Enumerating Keys and Safe Data Retrieval ---\n");
    
    pv_ContextEnumeratorHandle ctxEnum;
    result = pvBeginEnumerateContext(ctx, &ctxEnum);
    if (result == PV_RESULT_OK) {
        char* name;
        size_t nameSizeBytes;
        void* enumData;  // We'll ignore this unsafe pointer
        size_t enumDataSize;

        while (pvEnumerateContext(ctxEnum, &name, &nameSizeBytes, &enumData, &enumDataSize) == PV_RESULT_OK) {
            printf("Found key: %.*s\n", (int)nameSizeBytes, name);
            
            // Use safe pvGetContextData to retrieve the actual data
            size_t required_size = 0;
            pv_Result getResult = pvGetContextData(ctx, name, nameSizeBytes, nullptr, &required_size);
            if (getResult == PV_RESULT_OK) {
                // Allocate buffer and get data safely
                void* safe_buffer = malloc(required_size);
                size_t buffer_size = required_size;
                getResult = pvGetContextData(ctx, name, nameSizeBytes, safe_buffer, &buffer_size);
                
                if (getResult == PV_RESULT_OK) {
                    // Display the data based on the key name
                    if (strncmp(name, "app_name", nameSizeBytes) == 0 || 
                        strncmp(name, "version", nameSizeBytes) == 0) {
                        printf("  String value: %.*s\n", (int)buffer_size, (char*)safe_buffer);
                    }
                    else if (strncmp(name, "max_connections", nameSizeBytes) == 0) {
                        printf("  Integer value: %d\n", *(int*)safe_buffer);
                    }
                    else if (strncmp(name, "timeout", nameSizeBytes) == 0) {
                        printf("  Double value: %.1f\n", *(double*)safe_buffer);
                    }
                }
                free(safe_buffer);
            }
        }
        
        pvEndEnumerateContext(ctxEnum);
    }

    // Example 3: Demonstrate buffer size validation
    printf("\n--- Buffer Size Validation ---\n");
    
    // Demonstrate buffer too small error
    printf("Testing with insufficient buffer size:\n");
    char small_buffer[3];
    size_t small_size = sizeof(small_buffer);
    result = pvGetContextData(ctx, (char*)"app_name", strlen("app_name"), small_buffer, &small_size);
    if (result == PV_RESULT_INVALID_ARGUMENTS) {
        printf("Buffer too small! Provided 3 bytes, but need %zu bytes\n", small_size);
    }

    // Example 4: Modify existing data
    printf("\n--- Updating Configuration ---\n");
    
    int newMaxConnections = 200;
    pvSetContextData(ctx, (char*)"max_connections", strlen("max_connections"), 
                     &newMaxConnections, sizeof(newMaxConnections));
    printf("Updated max connections from %d to %d\n", maxConnections, newMaxConnections);

    // Example 5: Remove data that's no longer needed
    printf("\n--- Cleaning Up ---\n");
    
    result = pvUnsetContextData(ctx, (char*)"timeout", strlen("timeout"));
    if (result == PV_RESULT_OK) {
        printf("Removed timeout setting\n");
    }

    // Try to remove something that doesn't exist
    result = pvUnsetContextData(ctx, (char*)"nonexistent", strlen("nonexistent"));
    if (result == PV_RESULT_NOT_SUPPORTED) {
        printf("Tried to remove 'nonexistent' key - not found (as expected)\n");
    }

    // Clean up resources
    pvDestroyContext(ctx);
    if (logger) pvDestroyLogger(libHdl, logger);
    pvLibShutdown(libHdl);

    return 0;
}