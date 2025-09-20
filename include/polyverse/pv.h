#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
    #define pv_EXPORT extern "C" __attribute__((visibility("default")))
  #else
   #define pv_EXPORT __attribute__((visibility("default")))
#endif

enum pv_Result_t {
  PV_RESULT_INVALID_ARGUMENTS = -5,
  PV__RESULT_NOT_IMPLEMENTED = -4,
  PV_RESULT_NOT_SUPPORTED = -3,
  PV_RESULT_CONTEXT_ERROR = -2,
  PV_RESULT_UNKNOWN_ERROR = -1,
  PV_RESULT_OK = 0,
  PV_COMPLETE = 1
};
typedef enum pv_Result_t pv_Result;

#pragma region Result API

pv_EXPORT pv_Result pvGetResultString(pv_Result result, char* buf, size_t* bufSizeBytes);

#pragma endregion Result API

#pragma region Library API

struct pv_Lib_t;
typedef struct pv_Lib_t* pv_Lib;

typedef pv_Result (*pv_LibInitFunc)(pv_Lib*);
#define pv_InitFuncName "LibInit"
pv_EXPORT pv_Result pvLibInit(pv_Lib* libHdl);

typedef pv_Result (*pv_LibShutdownFunc)(pv_Lib libHdl);
#define pv_ShutdownFuncName "LibShutdown"
pv_EXPORT pv_Result pvLibShutdown(pv_Lib libHdl);

#pragma endregion Library API

#pragma region Logging API

struct pv_LoggerHandle_t;
typedef struct pv_LoggerHandle_t* pv_LoggerHandle;

enum pv_LogLevel_t {
  PV_LOGLEVEL_NONE = 0,
  PV__LOGLEVEL_FATAL = 1,
  PV_LOGLEVEL_ERROR = 2,
  PV_LOGLEVEL_WARNING = 3,
  PV_LOGLEVEL_INFO = 4,
  PV_LOGLEVEL_DEBUG = 5,
  PV_LOGLEVEL_VERBOSE = 6
};
typedef enum pv_LogLevel_t pv_LogLevel;

typedef void (*pv_OnLogFunc)(pv_LogLevel level, const char* function, size_t line, const char* file, const char* message);

struct pv_LoggerParams_t {
  pv_LogLevel logLevel;
  pv_OnLogFunc onLog;
};
typedef struct pv_LoggerParams_t pv_LoggerParams;

typedef pv_Result (*pv_CreateLoggerFunc)(pv_Lib libHdl, pv_LoggerParams params, pv_LoggerHandle* logger);
#define pv_CreateLoggerFuncName "CreateLogger"
pv_EXPORT pv_Result pvCreateLogger(pv_Lib libHdl, pv_LoggerParams params, pv_LoggerHandle* logger);

typedef pv_Result (*pv_LogFunc)(pv_LogLevel level, const char* function, size_t line, const char* file, const char* message);
#define pv_LogFuncName "Log"
pv_EXPORT pv_Result pvLog(pv_LogLevel level, const char* function, size_t line, const char* file, const char* message);

typedef pv_Result (*pv_DestroyLoggerFunc)(pv_Lib libHdl, pv_LoggerHandle logger);
#define pv_DestroyLoggerFuncName "DestroyLogger"
pv_EXPORT pv_Result pvDestroyLogger(pv_Lib libHdl, pv_LoggerHandle logger);

#pragma endregion Logging API

#pragma region Context API

struct pv_Context_t;
typedef struct pv_Context_t* pv_Context;

typedef pv_Result (*pv_CreateContextFunc)(pv_Context* ctx);
#define pv_CreateContextFuncName "CreateContext"
pv_EXPORT pv_Result pvCreateContext(pv_Context* ctx);

typedef pv_Result (*pv_DestroyContextFunc)(pv_Context ctx);
#define pv_DestroyContextFuncName "DestroyContext"
pv_EXPORT pv_Result pvDestroyContext(pv_Context ctx);

typedef pv_Result (*pv_GetContextDataFunc)(pv_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t* ctxDataSizeBytes);
#define pv_GetContextDataFuncName "GetContextData"
pv_EXPORT pv_Result pvGetContextData(pv_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t* ctxDataSizeBytes);

typedef pv_Result (*pv_SetContextDataFunc)(pv_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t ctxDataSizeBytes);
#define pv_SetContextDataFuncName "SetContextData"
pv_EXPORT pv_Result pvSetContextData(pv_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t ctxDataSizeBytes);

typedef pv_Result (*pv_UnsetContextDataFunc)(pv_Context ctx, char* name, size_t nameSizeBytes);
#define pv_UnsetContextDataFuncName "UnsetContextData"
pv_EXPORT pv_Result pvUnsetContextData(pv_Context ctx, char* name, size_t nameSizeBytes);

struct pv_ContextEnumeratorHandle_t;
typedef struct pv_ContextEnumeratorHandle_t* pv_ContextEnumeratorHandle;

typedef pv_Result (*pv_BeginEnumerateContextFunc)(pv_Context ctx, pv_ContextEnumeratorHandle* ctxEnum);
#define pv_BeginEnumerateContextFuncName "BeginEnumerateContext"
pv_EXPORT pv_Result pvBeginEnumerateContext(pv_Context ctx, pv_ContextEnumeratorHandle* ctxEnum);

typedef pv_Result (*pv_EnumerateContextFunc)(pv_ContextEnumeratorHandle ctxEnum, char** name, size_t* nameSizeBytes, void** data, size_t* dataSizeBytes);
#define pv_EnumerateContextFuncName "EnumerateContext"
pv_EXPORT pv_Result pvEnumerateContext(pv_ContextEnumeratorHandle ctxEnum, char** name, size_t* nameSizeBytes, void** data, size_t* dataSizeBytes);

typedef pv_Result (*pv_EndEnumerateContextFunc)(pv_ContextEnumeratorHandle ctxEnum);
#define pv_EndEnumerateContextFuncName "EndEnumerateContext"
pv_EXPORT pv_Result pvEndEnumerateContext(pv_ContextEnumeratorHandle ctxEnum);

#pragma endregion Context API

#pragma region Portal API

struct pv_Portal_t;
typedef struct pv_Portal_t* pv_Portal;

struct pv_PortalParams_t {
  char* uId;
  size_t uIdSizeBytes;
  bool server;
  pv_Context* context;
};
typedef struct pv_PortalParams_t pv_PortalParams;



typedef pv_Result (*pv_CreatePortalFunc)(pv_PortalParams params, pv_Portal* portal);
#define pv_CreatePortalFuncName "CreatePortal"
pv_EXPORT pv_Result pvCreatePortal(pv_PortalParams params, pv_Portal* portal);

typedef pv_Result (*pv_ReleasePortalFunc)(pv_Portal portal);
#define pv_ReleasePortalFuncName "ReleasePortal"
pv_EXPORT pv_Result pvReleasePortal(pv_Portal portal);

// struct pv_Node_t;
// typedef struct pv_Node_t* pv_Node;

// #pragma endregion Portal API

// #pragma region Node API

// typedef pv_Result (*pv_CreateNodeFunc)(pv_Portal portal, const char* id, size_t idSizeBytes, pv_Node* node);
// #define pv_CreateNodeFuncName "CreateNode"
// pv_EXPORT pv_Result pvCreateNode(pv_Portal portal, const char* id, size_t idSizeBytes, pv_Node* node);

// typedef pv_Result (*pv_AcquireNodeFunc)(pv_Portal portal, pv_Node node, pv_Context context, pv_Node* transformed);
// #define pv_AcquireNodeFuncName "AcquireNode"
// pv_EXPORT pv_Result pvAcquireNode(pv_Portal portal, pv_Node node, pv_Context context, pv_Node* transformed);

// typedef pv_Result (*pv_ReleaseNodeFunc)(pv_Node node);
// #define pv_ReleaseNodeFuncName "ReleaseNode"
// pv_EXPORT pv_Result pvReleaseNode(pv_Node node);

// typedef pv_Result (*pv_GetNodePropertyFunc)(pv_Node node, const char* propertyName, size_t propertyNameSizeBytes, pv_Context* properties);
// #define pv_GetNodePropertyFuncName "GetNodeProperty"
// pv_EXPORT pv_Result pvGetNodeProperty(pv_Node node, const char* propertyName, size_t propertyNameSizeBytes, pv_Context* properties);

// #pragma endregion Node API

// #pragma region Event API

// struct pv_Event_t;
// typedef struct pv_Event_t* pv_Event;

// #pragma endregion Event API

// #pragma region Queue API

// enum pv_QueueFlags_t {
//   pv_QUEUE_DEFAULT = 0,
//   pv_QUEUE_OUT_OF_ORDER_ENABLE = 1 >> 1,
//   pv_QUEUE_PROFILING_ENABLE = 1 >> 2
// };
// typedef enum pv_QueueFlags_t pv_QueueFlags;

// struct pv_QueueProperties_t {
//   pv_QueueFlags flags;
//   int32_t max_tries;
// };
// typedef struct pv_QueueProperties_t pv_QueueProperties;

// struct pv_Queue_t;
// typedef struct pv_Queue_t* pv_Queue;

// typedef pv_Result (*pv_CreateQueueFunc)(pv_Portal portal, pv_QueueProperties properties, pv_Queue* queue);
// #define pv_CreateQueueFuncName "CreateQueue"
// pv_EXPORT pv_Result pvCreateQueue(pv_Portal portal, pv_QueueProperties properties, pv_Queue* queue);

// typedef pv_Result (*pv_ReleaseQueueFunc)(pv_Queue queue);
// #define pv_ReleaseQueueFuncName "ReleaseQueue"
// pv_EXPORT pv_Result pvReleaseQueue(pv_Queue queue);

// #pragma endregion Queue API

// #pragma region Memory API

// struct pv_Mem_t;
// typedef struct pv_Mem_t* pv_Mem;

// enum pv_DeviceType_t {
//   pv_CPU = 1,
//   pv_CUDA = 2,
//   pv_CUDAHost = 3,
//   pv_OpenCL = 4,
//   pv_Vulkan = 7,
//   pv_Metal = 8,
//   pv_VPI = 9,
//   pv_ROCM = 10,
//   pv_ROCMHost = 11,
//   pv_ExtDev = 12,
//   pv_CUDAManaged = 13,
//   pv_OneAPI = 14,
//   pv_WebGPU = 15,
//   pv_Hexagon = 16,
// };
// typedef enum pv_DeviceType_t pv_DeviceType;

// enum pv_ElemType_t {
//   pv_Int = 0U,
//   pv_UInt = 1U,
//   pv_Float = 2U,
//   pv_OpaqueHandle = 3U,
//   pv_Bfloat = 4U,
//   pv_Complex = 5U,
// };
// typedef enum pv_ElemType_t pv_ElemType;

// struct pv_Datatype_t {
//   pv_ElemType code;
//   uint8_t bits;
//   uint16_t lanes;
// };
// typedef struct pv_Datatype_t pv_Datatype;

// struct pv_MemParams_t
// {
//   pv_DeviceType deviceType;
//   int32_t dim;
//   int64_t* shape;
//   int64_t* strides;
//   uint64_t byte_offset;
// };
// typedef struct pv_MemParams_t pv_MemParams;

// typedef pv_Result (*pv_CreateMemFunc)(pv_Portal portal, pv_MemParams params, void* data, pv_Mem* mem);
// #define pv_CreateMemFuncName "CreateMem"
// pv_EXPORT pv_Result pvCreateMem(pv_Portal portal, pv_MemParams params, void* data, pv_Mem* mem);

// typedef pv_Result (*pv_ReleaseMemFunc)(pv_Mem mem);
// #define pv_ReleaseMemFuncName "ReleaseMem"
// pv_EXPORT pv_Result pvReleaseMem(pv_Mem mem);

// #pragma endregion Memory API

// #pragma region Execution API

// typedef pv_Result (*pv_EnqueueWriteMemFunc)(pv_Queue queue, pv_Mem mem, bool blocking_write, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, pv_Event* event_wait_list, pv_Event* event);
// #define pv_EnqueueWriteMemFuncName "EnqueueWriteMem"
// pv_EXPORT pv_Result pvEnqueueWriteMem(pv_Queue queue, pv_Mem mem, bool blocking_write, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, pv_Event* event_wait_list, pv_Event* event);

// typedef pv_Result (*pv_EnqueueReadMemFunc)(pv_Queue queue, pv_Mem mem, bool blocking_read, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, pv_Event* event_wait_list, pv_Event* event);
// #define pv_EnqueueReadMemFuncName "EnqueueReadMem"
// pv_EXPORT pv_Result pvEnqueueReadMem(pv_Queue queue, pv_Mem mem, bool blocking_read, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, pv_Event* event_wait_list, pv_Event* event);

// typedef pv_Result (*pv_EnqueueStepFunc)(pv_Queue queue, pv_Node node, pv_Context context, uint32_t num_events_in_wait_list, pv_Event* event_wait_list, pv_Event* event);
// #define pv_EnqueueStepFuncName "EnqueueStep"
// pv_EXPORT pv_Result pvEnqueueStep(pv_Queue queue, pv_Node node, pv_Context context, uint32_t num_events_in_wait_list, pv_Event* event_wait_list, pv_Event* event);

// typedef pv_Result (*pv_FlushFunc)(pv_Queue queue);
// #define pv_FlushFuncName "Flush"
// pv_EXPORT pv_Result pvFlush(pv_Queue queue);

// typedef pv_Result (*pv_FinishFunc)(pv_Queue queue);
// #define pv_FinishFuncName "Finish"
// pv_EXPORT pv_Result pvFinish(pv_Queue queue);

// #pragma endregion Execution API