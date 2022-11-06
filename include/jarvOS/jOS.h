#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
    #define jOS_EXPORT extern "C" __attribute__((visibility("default")))
  #else
   #define jOS_EXPORT __attribute__((visibility("default")))
#endif

enum jOS_Result_t {
  jOS_RESULT_INVALID_ARGUMENTS = -5,
  jOS_RESULT_NOT_IMPLEMENTED = -4,
  jOS_RESULT_NOT_SUPPORTED = -3,
  jOS_RESULT_MODULE_NOT_FOUND = -2,
  jOS_RESULT_UNKNOWN_ERROR = -1,
  jOS_RESULT_OK = 0,
  jOS_COMPLETE = 1
};
typedef enum jOS_Result_t jOS_Result;

#pragma region Library API

struct jOS_Lib_t;
typedef struct jOS_Lib_t* jOS_Lib;

typedef jOS_Result (*jOS_LibInitFunc)(jOS_Lib*);
#define jOS_InitFuncName "LibInit"
jOS_EXPORT jOS_Result jOSLibInit(jOS_Lib* libHdl);

typedef jOS_Result (*jOS_LibShutdownFunc)(jOS_Lib libHdl);
#define jOS_ShutdownFuncName "LibShutdown"
jOS_EXPORT jOS_Result jOSLibShutdown(jOS_Lib libHdl);

#pragma endregion Library API

#pragma region Logging API

struct jOS_LoggerHandle_t;
typedef struct jOS_LoggerHandle_t* jOS_LoggerHandle;

enum jOS_LogLevel_t {
  jOS_LOGLEVEL_NONE = 0,
  jOS_LOGLEVEL_FATAL = 1,
  jOS_LOGLEVEL_ERROR = 2,
  jOS_LOGLEVEL_WARNING = 3,
  jOS_LOGLEVEL_INFO = 4,
  jOS_LOGLEVEL_DEBUG = 5,
  jOS_LOGLEVEL_VERBOSE = 6
};
typedef enum jOS_LogLevel_t jOS_LogLevel;

typedef void (*jOS_OnLogFunc)(jOS_LogLevel level, const char* function, size_t line, const char* file, const char* message);

struct jOS_LoggerParams_t {
  jOS_LogLevel logLevel;
  jOS_OnLogFunc onLog;
};
typedef struct jOS_LoggerParams_t jOS_LoggerParams;

typedef jOS_Result (*jOS_CreateLoggerFunc)(jOS_Lib libHdl, jOS_LoggerParams params, jOS_LoggerHandle* logger);
#define jOS_CreateLoggerFuncName "CreateLogger"
jOS_EXPORT jOS_Result jOSCreateLogger(jOS_Lib libHdl, jOS_LoggerParams params, jOS_LoggerHandle* logger);

typedef jOS_Result (*jOS_LogFunc)(jOS_LogLevel level, const char* function, size_t line, const char* file, const char* message);
#define jOS_LogFuncName "Log"
jOS_EXPORT jOS_Result jOSLog(jOS_LogLevel level, const char* function, size_t line, const char* file, const char* message);

typedef jOS_Result (*jOS_DestroyLoggerFunc)(jOS_Lib libHdl, jOS_LoggerHandle logger);
#define jOS_DestroyLoggerFuncName "DestroyLogger"
jOS_EXPORT jOS_Result jOSDestroyLogger(jOS_Lib libHdl, jOS_LoggerHandle logger);

#pragma endregion Logging API

#pragma region Module API

struct jOS_Module_t;
typedef struct jOS_Module_t* jOS_Module;

struct jOS_ModuleParams_t
{
  char* name;
  size_t nameSizeBytes;

  char* hintPath;
  size_t hintPathSizeBytes;
};
typedef struct jOS_ModuleParams_t jOS_ModuleParams;

typedef jOS_Result (*jOS_AcquireModuleFunc)(jOS_ModuleParams params, jOS_Module* jOSm);
#define jOS_AcquireModuleFuncName "AcquireModule"
jOS_EXPORT jOS_Result jOSAcquireModule(jOS_ModuleParams params, jOS_Module* jOSm);

typedef jOS_Result (*jOS_ReleaseModuleFunc)(jOS_Module jOSm);
#define jOS_ReleaseModuleFuncName "ReleaseModule"
jOS_EXPORT jOS_Result jOSReleaseModule(jOS_Module jOSm);

typedef jOS_Result (*jOS_InitModuleFunc)();
#define jOS_InitModuleFuncName "InitModule"

typedef jOS_Result (*jOS_shutdownModuleFunc)();
#define jOS_shutdownModuleFuncName "shutdownModule"

#pragma endregion Module API

#pragma region Context API

// TODO: Add Module argument here, contexts are specific to modules
struct jOS_Context_t;
typedef struct jOS_Context_t* jOS_Context;

typedef jOS_Result (*jOS_CreateContextFunc)(jOS_Context* ctx);
#define jOS_CreateContextFuncName "CreateContext"
jOS_EXPORT jOS_Result jOSCreateContext(jOS_Context* ctx);

typedef jOS_Result (*jOS_DestroyContextFunc)(jOS_Context ctx);
#define jOS_DestroyContextFuncName "DestroyContext"
jOS_EXPORT jOS_Result jOSDestroyContext(jOS_Context ctx);

typedef jOS_Result (*jOS_SetContextDataFunc)(jOS_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t ctxDataSizeBytes);
#define jOS_SetContextDataFuncName "SetContextData"
jOS_EXPORT jOS_Result jOSSetContextData(jOS_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t ctxDataSizeBytes);

typedef jOS_Result (*jOS_UnsetContextDataFunc)(jOS_Context ctx, char* name, size_t nameSizeBytes);
#define jOS_UnsetContextDataFuncName "UnsetContextData"
jOS_EXPORT jOS_Result jOSUnsetContextData(jOS_Context ctx, char* name, size_t nameSizeBytes);

struct jOS_ContextEnumeratorHandle_t;
typedef struct jOS_ContextEnumeratorHandle_t* jOS_ContextEnumeratorHandle;

typedef jOS_Result (*jOS_BeginEnumerateContextFunc)(jOS_Context ctx, jOS_ContextEnumeratorHandle* ctxEnum);
#define jOS_BeginEnumerateContextFuncName "BeginEnumerateContext"
jOS_EXPORT jOS_Result jOSBeginEnumerateContext(jOS_Context ctx, jOS_ContextEnumeratorHandle* ctxEnum);

typedef jOS_Result (*jOS_EnumerateContextFunc)(jOS_ContextEnumeratorHandle ctxEnum, char** name, size_t* nameSizeBytes, void** data, size_t* dataSizeBytes);
#define jOS_EnumerateContextFuncName "EnumerateContext"
jOS_EXPORT jOS_Result jOSEnumerateContext(jOS_ContextEnumeratorHandle ctxEnum, char** name, size_t* nameSizeBytes, void** data, size_t* dataSizeBytes);

typedef jOS_Result (*jOS_EndEnumerateContextFunc)(jOS_ContextEnumeratorHandle ctxEnum);
#define jOS_EndEnumerateContextFuncName "EndEnumerateContext"
jOS_EXPORT jOS_Result jOSEndEnumerateContext(jOS_ContextEnumeratorHandle ctxEnum);

#pragma endregion Context API

#pragma region Portal API

struct jOS_Portal_t;
typedef struct jOS_Portal_t* jOS_Portal;

struct jOS_PortalParams_t {
  char* id;
  size_t idSizeBytes;
};
typedef struct jOS_PortalParams_t jOS_PortalParams;

typedef jOS_Result (*jOS_CreatePortalFunc)(jOS_Module jOSm, jOS_PortalParams params, jOS_Portal* portal);
#define jOS_CreatePortalFuncName "CreatePortal"
jOS_EXPORT jOS_Result jOSCreatePortal(jOS_Module jOSm, jOS_PortalParams params, jOS_Portal* portal);

typedef jOS_Result (*jOS_ReleasePortalFunc)(jOS_Portal portal);
#define jOS_ReleasePortalFuncName "ReleasePortal"
jOS_EXPORT jOS_Result jOSReleasePortal(jOS_Portal portal);

typedef jOS_Result (*jOS_GetPortalPropertyFunc)(jOS_Portal portal, char* propertyName, size_t propertyNameSizeBytes, jOS_Context* properties);
#define jOS_GetPortalPropertyFuncName "GetPortalProperty"
jOS_EXPORT jOS_Result jOSGetPortalProperty(jOS_Portal portal, char* propertyName, size_t propertyNameSizeBytes, jOS_Context* properties);

struct jOS_Node_t;
typedef struct jOS_Node_t* jOS_Node;

#pragma endregion Portal API

#pragma region Node API

typedef jOS_Result (*jOS_CreateNodeFunc)(jOS_Portal portal, const char* id, size_t idSizeBytes, jOS_Node* node);
#define jOS_CreateNodeFuncName "CreateNode"
jOS_EXPORT jOS_Result jOSCreateNode(jOS_Portal portal, const char* id, size_t idSizeBytes, jOS_Node* node);

typedef jOS_Result (*jOS_AcquireNodeFunc)(jOS_Portal portal, jOS_Node node, jOS_Context context, jOS_Node* transformed);
#define jOS_AcquireNodeFuncName "AcquireNode"
jOS_EXPORT jOS_Result jOSAcquireNode(jOS_Portal portal, jOS_Node node, jOS_Context context, jOS_Node* transformed);

typedef jOS_Result (*jOS_ReleaseNodeFunc)(jOS_Node node);
#define jOS_ReleaseNodeFuncName "ReleaseNode"
jOS_EXPORT jOS_Result jOSReleaseNode(jOS_Node node);

typedef jOS_Result (*jOS_GetNodePropertyFunc)(jOS_Node node, const char* propertyName, size_t propertyNameSizeBytes, jOS_Context* properties);
#define jOS_GetNodePropertyFuncName "GetNodeProperty"
jOS_EXPORT jOS_Result jOSGetNodeProperty(jOS_Node node, const char* propertyName, size_t propertyNameSizeBytes, jOS_Context* properties);

#pragma endregion Node API

#pragma region Event API

struct jOS_Event_t;
typedef struct jOS_Event_t* jOS_Event;

#pragma endregion Event API

#pragma region Queue API

enum jOS_QueueFlags_t {
  jOS_QUEUE_DEFAULT = 0,
  jOS_QUEUE_OUT_OF_ORDER_ENABLE = 1 >> 1,
  jOS_QUEUE_PROFILING_ENABLE = 1 >> 2
};
typedef enum jOS_QueueFlags_t jOS_QueueFlags;

struct jOS_QueueProperties_t {
  jOS_QueueFlags flags;
  int32_t max_tries;
};
typedef struct jOS_QueueProperties_t jOS_QueueProperties;

struct jOS_Queue_t;
typedef struct jOS_Queue_t* jOS_Queue;

typedef jOS_Result (*jOS_CreateQueueFunc)(jOS_Portal portal, jOS_QueueProperties properties, jOS_Queue* queue);
#define jOS_CreateQueueFuncName "CreateQueue"
jOS_EXPORT jOS_Result jOSCreateQueue(jOS_Portal portal, jOS_QueueProperties properties, jOS_Queue* queue);

typedef jOS_Result (*jOS_ReleaseQueueFunc)(jOS_Queue queue);
#define jOS_ReleaseQueueFuncName "ReleaseQueue"
jOS_EXPORT jOS_Result jOSReleaseQueue(jOS_Queue queue);

#pragma endregion Queue API

#pragma region Memory API

struct jOS_Mem_t;
typedef struct jOS_Mem_t* jOS_Mem;

enum jOS_DeviceType_t {
  jOS_CPU = 1,
  jOS_CUDA = 2,
  jOS_CUDAHost = 3,
  jOS_OpenCL = 4,
  jOS_Vulkan = 7,
  jOS_Metal = 8,
  jOS_VPI = 9,
  jOS_ROCM = 10,
  jOS_ROCMHost = 11,
  jOS_ExtDev = 12,
  jOS_CUDAManaged = 13,
  jOS_OneAPI = 14,
  jOS_WebGPU = 15,
  jOS_Hexagon = 16,
};
typedef enum jOS_DeviceType_t jOS_DeviceType;

enum jOS_ElemType_t {
  jOS_Int = 0U,
  jOS_UInt = 1U,
  jOS_Float = 2U,
  jOS_OpaqueHandle = 3U,
  jOS_Bfloat = 4U,
  jOS_Complex = 5U,
};
typedef enum jOS_ElemType_t jOS_ElemType;

struct jOS_Datatype_t {
  jOS_ElemType code;
  uint8_t bits;
  uint16_t lanes;
};
typedef struct jOS_Datatype_t jOS_Datatype;

struct jOS_MemParams_t
{
  jOS_DeviceType deviceType;
  int32_t dim;
  int64_t* shape;
  int64_t* strides;
  uint64_t byte_offset;
};
typedef struct jOS_MemParams_t jOS_MemParams;

typedef jOS_Result (*jOS_CreateMemFunc)(jOS_Portal portal, jOS_MemParams params, void* data, jOS_Mem* mem);
#define jOS_CreateMemFuncName "CreateMem"
jOS_EXPORT jOS_Result jOSCreateMem(jOS_Portal portal, jOS_MemParams params, void* data, jOS_Mem* mem);

typedef jOS_Result (*jOS_ReleaseMemFunc)(jOS_Mem mem);
#define jOS_ReleaseMemFuncName "ReleaseMem"
jOS_EXPORT jOS_Result jOSReleaseMem(jOS_Mem mem);

#pragma endregion Memory API

#pragma region Execution API

typedef jOS_Result (*jOS_EnqueueWriteMemFunc)(jOS_Queue queue, jOS_Mem mem, bool blocking_write, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, jOS_Event* event_wait_list, jOS_Event* event);
#define jOS_EnqueueWriteMemFuncName "EnqueueWriteMem"
jOS_EXPORT jOS_Result jOSEnqueueWriteMem(jOS_Queue queue, jOS_Mem mem, bool blocking_write, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, jOS_Event* event_wait_list, jOS_Event* event);

typedef jOS_Result (*jOS_EnqueueReadMemFunc)(jOS_Queue queue, jOS_Mem mem, bool blocking_read, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, jOS_Event* event_wait_list, jOS_Event* event);
#define jOS_EnqueueReadMemFuncName "EnqueueReadMem"
jOS_EXPORT jOS_Result jOSEnqueueReadMem(jOS_Queue queue, jOS_Mem mem, bool blocking_read, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, jOS_Event* event_wait_list, jOS_Event* event);

typedef jOS_Result (*jOS_EnqueueStepFunc)(jOS_Queue queue, jOS_Node node, jOS_Context context, uint32_t num_events_in_wait_list, jOS_Event* event_wait_list, jOS_Event* event);
#define jOS_EnqueueStepFuncName "EnqueueStep"
jOS_EXPORT jOS_Result jOSEnqueueStep(jOS_Queue queue, jOS_Node node, jOS_Context context, uint32_t num_events_in_wait_list, jOS_Event* event_wait_list, jOS_Event* event);

typedef jOS_Result (*jOS_FlushFunc)(jOS_Queue queue);
#define jOS_FlushFuncName "Flush"
jOS_EXPORT jOS_Result jOSFlush(jOS_Queue queue);

typedef jOS_Result (*jOS_FinishFunc)(jOS_Queue queue);
#define jOS_FinishFuncName "Finish"
jOS_EXPORT jOS_Result jOSFinish(jOS_Queue queue);

#pragma endregion Execution API