#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
    #define ZP_EXPORT extern "C" __attribute__((visibility("default")))
  #else
   #define ZP_EXPORT __attribute__((visibility("default")))
#endif

enum ZP_Result_t {
  ZP_RESULT_INVALID_ARGUMENTS = -5,
  ZP_RESULT_NOT_IMPLEMENTED = -4,
  ZP_RESULT_NOT_SUPPORTED = -3,
  ZP_RESULT_MODULE_NOT_FOUND = -2,
  ZP_RESULT_UNKNOWN_ERROR = -1,
  ZP_RESULT_OK = 0,
  ZP_COMPLETE = 1
};
typedef enum ZP_Result_t ZP_Result;

#pragma region Library API

struct ZP_Lib_t;
typedef struct ZP_Lib_t* ZP_Lib;

typedef ZP_Result (*ZP_LibInitFunc)(ZP_Lib*);
#define ZP_InitFuncName "LibInit"
ZP_EXPORT ZP_Result zpLibInit(ZP_Lib* libHdl);

typedef ZP_Result (*ZP_LibShutdownFunc)(ZP_Lib libHdl);
#define ZP_ShutdownFuncName "LibShutdown"
ZP_EXPORT ZP_Result zpLibShutdown(ZP_Lib libHdl);

#pragma endregion Library API

#pragma region Logging API

struct ZP_LoggerHandle_t;
typedef struct ZP_LoggerHandle_t* ZP_LoggerHandle;

enum ZP_LogLevel_t {
  ZP_LOGLEVEL_NONE = 0,
  ZP_LOGLEVEL_FATAL = 1,
  ZP_LOGLEVEL_ERROR = 2,
  ZP_LOGLEVEL_WARNING = 3,
  ZP_LOGLEVEL_INFO = 4,
  ZP_LOGLEVEL_DEBUG = 5,
  ZP_LOGLEVEL_VERBOSE = 6
};
typedef enum ZP_LogLevel_t ZP_LogLevel;

typedef void (*ZP_OnLogFunc)(ZP_LogLevel level, const char* function, size_t line, const char* file, const char* message);

struct ZP_LoggerParams_t {
  ZP_LogLevel logLevel;
  ZP_OnLogFunc onLog;
};
typedef struct ZP_LoggerParams_t ZP_LoggerParams;

typedef ZP_Result (*ZP_CreateLoggerFunc)(ZP_Lib libHdl, ZP_LoggerParams params, ZP_LoggerHandle* logger);
#define ZP_CreateLoggerFuncName "CreateLogger"
ZP_EXPORT ZP_Result zpCreateLogger(ZP_Lib libHdl, ZP_LoggerParams params, ZP_LoggerHandle* logger);

typedef ZP_Result (*ZP_LogFunc)(ZP_LogLevel level, const char* function, size_t line, const char* file, const char* message);
#define ZP_LogFuncName "Log"
ZP_EXPORT ZP_Result zpLog(ZP_LogLevel level, const char* function, size_t line, const char* file, const char* message);

typedef ZP_Result (*ZP_DestroyLoggerFunc)(ZP_Lib libHdl, ZP_LoggerHandle logger);
#define ZP_DestroyLoggerFuncName "DestroyLogger"
ZP_EXPORT ZP_Result zpDestroyLogger(ZP_Lib libHdl, ZP_LoggerHandle logger);

#pragma endregion Logging API

#pragma region Module API

struct ZP_Module_t;
typedef struct ZP_Module_t* ZP_Module;

struct ZP_ModuleParams_t
{
  char* name;
  size_t nameSizeBytes;

  char* hintPath;
  size_t hintPathSizeBytes;
};
typedef struct ZP_ModuleParams_t ZP_ModuleParams;

typedef ZP_Result (*ZP_AcquireModuleFunc)(ZP_ModuleParams params, ZP_Module* zpm);
#define ZP_AcquireModuleFuncName "AcquireModule"
ZP_EXPORT ZP_Result zpAcquireModule(ZP_ModuleParams params, ZP_Module* zpm);

typedef ZP_Result (*ZP_ReleaseModuleFunc)(ZP_Module zpm);
#define ZP_ReleaseModuleFuncName "ReleaseModule"
ZP_EXPORT ZP_Result zpReleaseModule(ZP_Module zpm);

typedef ZP_Result (*ZP_InitModuleFunc)();
#define ZP_InitModuleFuncName "InitModule"

typedef ZP_Result (*ZP_shutdownModuleFunc)();
#define ZP_shutdownModuleFuncName "shutdownModule"

#pragma endregion Module API

#pragma region Context API

// TODO: Add Module argument here, contexts are specific to modules
struct ZP_Context_t;
typedef struct ZP_Context_t* ZP_Context;

typedef ZP_Result (*ZP_CreateContextFunc)(ZP_Context* ctx);
#define ZP_CreateContextFuncName "CreateContext"
ZP_EXPORT ZP_Result zpCreateContext(ZP_Context* ctx);

typedef ZP_Result (*ZP_DestroyContextFunc)(ZP_Context ctx);
#define ZP_DestroyContextFuncName "DestroyContext"
ZP_EXPORT ZP_Result zpDestroyContext(ZP_Context ctx);

typedef ZP_Result (*ZP_SetContextDataFunc)(ZP_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t ctxDataSizeBytes);
#define ZP_SetContextDataFuncName "SetContextData"
ZP_EXPORT ZP_Result zpSetContextData(ZP_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t ctxDataSizeBytes);

typedef ZP_Result (*ZP_UnsetContextDataFunc)(ZP_Context ctx, char* name, size_t nameSizeBytes);
#define ZP_UnsetContextDataFuncName "UnsetContextData"
ZP_EXPORT ZP_Result zpUnsetContextData(ZP_Context ctx, char* name, size_t nameSizeBytes);

struct ZP_ContextEnumeratorHandle_t;
typedef struct ZP_ContextEnumeratorHandle_t* ZP_ContextEnumeratorHandle;

typedef ZP_Result (*ZP_BeginEnumerateContextFunc)(ZP_Context ctx, ZP_ContextEnumeratorHandle* ctxEnum);
#define ZP_BeginEnumerateContextFuncName "BeginEnumerateContext"
ZP_EXPORT ZP_Result zpBeginEnumerateContext(ZP_Context ctx, ZP_ContextEnumeratorHandle* ctxEnum);

typedef ZP_Result (*ZP_EnumerateContextFunc)(ZP_ContextEnumeratorHandle ctxEnum, char** name, size_t* nameSizeBytes, void** data, size_t* dataSizeBytes);
#define ZP_EnumerateContextFuncName "EnumerateContext"
ZP_EXPORT ZP_Result zpEnumerateContext(ZP_ContextEnumeratorHandle ctxEnum, char** name, size_t* nameSizeBytes, void** data, size_t* dataSizeBytes);

typedef ZP_Result (*ZP_EndEnumerateContextFunc)(ZP_ContextEnumeratorHandle ctxEnum);
#define ZP_EndEnumerateContextFuncName "EndEnumerateContext"
ZP_EXPORT ZP_Result zpEndEnumerateContext(ZP_ContextEnumeratorHandle ctxEnum);

#pragma endregion Context API

#pragma region Portal API

struct ZP_Portal_t;
typedef struct ZP_Portal_t* ZP_Portal;

struct ZP_PortalParams_t {
  char* id;
  size_t idSizeBytes;
};
typedef struct ZP_PortalParams_t ZP_PortalParams;

typedef ZP_Result (*ZP_CreatePortalFunc)(ZP_Module zpm, ZP_PortalParams params, ZP_Portal* portal);
#define ZP_CreatePortalFuncName "CreatePortal"
ZP_EXPORT ZP_Result zpCreatePortal(ZP_Module zpm, ZP_PortalParams params, ZP_Portal* portal);

typedef ZP_Result (*ZP_ReleasePortalFunc)(ZP_Portal portal);
#define ZP_ReleasePortalFuncName "ReleasePortal"
ZP_EXPORT ZP_Result zpReleasePortal(ZP_Portal portal);

typedef ZP_Result (*ZP_GetPortalPropertyFunc)(ZP_Portal portal, char* propertyName, size_t propertyNameSizeBytes, ZP_Context* properties);
#define ZP_GetPortalPropertyFuncName "GetPortalProperty"
ZP_EXPORT ZP_Result zpGetPortalProperty(ZP_Portal portal, char* propertyName, size_t propertyNameSizeBytes, ZP_Context* properties);

struct ZP_Node_t;
typedef struct ZP_Node_t* ZP_Node;

#pragma endregion Portal API

#pragma region Node API

typedef ZP_Result (*ZP_CreateNodeFunc)(ZP_Portal portal, const char* id, size_t idSizeBytes, ZP_Node* node);
#define ZP_CreateNodeFuncName "CreateNode"
ZP_EXPORT ZP_Result zpCreateNode(ZP_Portal portal, const char* id, size_t idSizeBytes, ZP_Node* node);

typedef ZP_Result (*ZP_AcquireNodeFunc)(ZP_Portal portal, ZP_Node node, ZP_Context context, ZP_Node* transformed);
#define ZP_AcquireNodeFuncName "AcquireNode"
ZP_EXPORT ZP_Result zpAcquireNode(ZP_Portal portal, ZP_Node node, ZP_Context context, ZP_Node* transformed);

typedef ZP_Result (*ZP_ReleaseNodeFunc)(ZP_Node node);
#define ZP_ReleaseNodeFuncName "ReleaseNode"
ZP_EXPORT ZP_Result zpReleaseNode(ZP_Node node);

typedef ZP_Result (*ZP_GetNodePropertyFunc)(ZP_Node node, const char* propertyName, size_t propertyNameSizeBytes, ZP_Context* properties);
#define ZP_GetNodePropertyFuncName "GetNodeProperty"
ZP_EXPORT ZP_Result zpGetNodeProperty(ZP_Node node, const char* propertyName, size_t propertyNameSizeBytes, ZP_Context* properties);

#pragma endregion Node API

#pragma region Event API

struct ZP_Event_t;
typedef struct ZP_Event_t* ZP_Event;

#pragma endregion Event API

#pragma region Queue API

enum ZP_QueueFlags_t {
  ZP_QUEUE_DEFAULT = 0,
  ZP_QUEUE_OUT_OF_ORDER_ENABLE = 1 >> 1,
  ZP_QUEUE_PROFILING_ENABLE = 1 >> 2
};
typedef enum ZP_QueueFlags_t ZP_QueueFlags;

struct ZP_QueueProperties_t {
  ZP_QueueFlags flags;
  int32_t max_tries;
};
typedef struct ZP_QueueProperties_t ZP_QueueProperties;

struct ZP_Queue_t;
typedef struct ZP_Queue_t* ZP_Queue;

typedef ZP_Result (*ZP_CreateQueueFunc)(ZP_Portal portal, ZP_QueueProperties properties, ZP_Queue* queue);
#define ZP_CreateQueueFuncName "CreateQueue"
ZP_EXPORT ZP_Result zpCreateQueue(ZP_Portal portal, ZP_QueueProperties properties, ZP_Queue* queue);

typedef ZP_Result (*ZP_ReleaseQueueFunc)(ZP_Queue queue);
#define ZP_ReleaseQueueFuncName "ReleaseQueue"
ZP_EXPORT ZP_Result zpReleaseQueue(ZP_Queue queue);

#pragma endregion Queue API

#pragma region Memory API

struct ZP_Mem_t;
typedef struct ZP_Mem_t* ZP_Mem;

enum ZP_DeviceType_t {
  ZP_CPU = 1,
  ZP_CUDA = 2,
  ZP_CUDAHost = 3,
  ZP_OpenCL = 4,
  ZP_Vulkan = 7,
  ZP_Metal = 8,
  ZP_VPI = 9,
  ZP_ROCM = 10,
  ZP_ROCMHost = 11,
  ZP_ExtDev = 12,
  ZP_CUDAManaged = 13,
  ZP_OneAPI = 14,
  ZP_WebGPU = 15,
  ZP_Hexagon = 16,
};
typedef enum ZP_DeviceType_t ZP_DeviceType;

enum ZP_ElemType_t {
  ZP_Int = 0U,
  ZP_UInt = 1U,
  ZP_Float = 2U,
  ZP_OpaqueHandle = 3U,
  ZP_Bfloat = 4U,
  ZP_Complex = 5U,
};
typedef enum ZP_ElemType_t ZP_ElemType;

struct ZP_Datatype_t {
  ZP_ElemType code;
  uint8_t bits;
  uint16_t lanes;
};
typedef struct ZP_Datatype_t ZP_Datatype;

struct ZP_MemParams_t
{
  ZP_DeviceType deviceType;
  int32_t dim;
  int64_t* shape;
  int64_t* strides;
  uint64_t byte_offset;
};
typedef struct ZP_MemParams_t ZP_MemParams;

typedef ZP_Result (*ZP_CreateMemFunc)(ZP_Portal portal, ZP_MemParams params, void* data, ZP_Mem* mem);
#define ZP_CreateMemFuncName "CreateMem"
ZP_EXPORT ZP_Result zpCreateMem(ZP_Portal portal, ZP_MemParams params, void* data, ZP_Mem* mem);

typedef ZP_Result (*ZP_ReleaseMemFunc)(ZP_Mem mem);
#define ZP_ReleaseMemFuncName "ReleaseMem"
ZP_EXPORT ZP_Result zpReleaseMem(ZP_Mem mem);

#pragma endregion Memory API

#pragma region Execution API

typedef ZP_Result (*ZP_EnqueueWriteMemFunc)(ZP_Queue queue, ZP_Mem mem, bool blocking_write, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);
#define ZP_EnqueueWriteMemFuncName "EnqueueWriteMem"
ZP_EXPORT ZP_Result zpEnqueueWriteMem(ZP_Queue queue, ZP_Mem mem, bool blocking_write, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);

typedef ZP_Result (*ZP_EnqueueReadMemFunc)(ZP_Queue queue, ZP_Mem mem, bool blocking_read, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);
#define ZP_EnqueueReadMemFuncName "EnqueueReadMem"
ZP_EXPORT ZP_Result zpEnqueueReadMem(ZP_Queue queue, ZP_Mem mem, bool blocking_read, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);

typedef ZP_Result (*ZP_EnqueueStepFunc)(ZP_Queue queue, ZP_Node node, ZP_Context context, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);
#define ZP_EnqueueStepFuncName "EnqueueStep"
ZP_EXPORT ZP_Result zpEnqueueStep(ZP_Queue queue, ZP_Node node, ZP_Context context, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);

typedef ZP_Result (*ZP_FlushFunc)(ZP_Queue queue);
#define ZP_FlushFuncName "Flush"
ZP_EXPORT ZP_Result zpFlush(ZP_Queue queue);

typedef ZP_Result (*ZP_FinishFunc)(ZP_Queue queue);
#define ZP_FinishFuncName "Finish"
ZP_EXPORT ZP_Result zpFinish(ZP_Queue queue);

#pragma endregion Execution API