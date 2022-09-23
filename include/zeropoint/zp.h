#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define CONST_STRING const char*
 
#ifdef __cplusplus
    #define ZP_EXPORT extern "C" __attribute__((visibility("default")))
  #else
   #define ZP_EXPORT __attribute__((visibility("default")))
#endif

enum ZP_Result_t {
  ZP_RESULT_NOT_SUPPORTED = -2,
  ZP_RESULT_NOT_IMPLEMENTED = -1,
  ZP_RESULT_OK = 0,
  ZP_COMPLETE = 1
};
typedef enum ZP_Result_t ZP_Result;

#pragma region Library API

struct ZP_Lib_t;
typedef struct ZP_Lib_t* ZP_Lib;

typedef ZP_Result (*ZP_InitFunc)(ZP_Lib*);
CONST_STRING ZP_InitFuncName = "Init";
ZP_EXPORT ZP_Result zpInit(ZP_Lib* libHdl);

typedef ZP_Result (*ZP_ShutdownFunc)(ZP_Lib libHdl);
CONST_STRING ZP_ShutdownFuncName = "Shutdown";
ZP_EXPORT ZP_Result zpShutdown(ZP_Lib libHdl);

#pragma endregion Library API

#pragma region Module API

struct ZP_Module_t;
typedef struct ZP_Module_t* ZP_Module;

struct ZP_ModuleParams_t
{
  char* id;
  size_t idSizeBytes;

  char* hintPath;
  size_t hintPathSizeBytes;
};
typedef struct ZP_ModuleParams_t* ZP_ModuleParams;

typedef ZP_Result (*ZP_AcquireModuleFunc)(ZP_ModuleParams params, ZP_Module* zpm);
CONST_STRING ZP_AcquireModuleFuncName = "AcquireModule";
ZP_EXPORT ZP_Result zpAcquireModule(ZP_ModuleParams params, ZP_Module* zpm);

typedef ZP_Result (*ZP_ReleaseModuleFunc)(ZP_Module zpm);
CONST_STRING ZP_ReleaseModuleFuncName = "ReleaseModule";
ZP_EXPORT ZP_Result zpReleaseModule(ZP_Module zpm);

#pragma endregion Module API

#pragma region Context API

struct ZP_Context_t;
typedef struct ZP_Context_t* ZP_Context;

typedef ZP_Result (*ZP_CreateContextFunc)(ZP_Context* ctx);
CONST_STRING ZP_CreateContextFuncName = "CreateContext";
ZP_EXPORT ZP_Result zpCreateContext(ZP_Context* ctx);

typedef ZP_Result (*ZP_DestroyContextFunc)(ZP_Context ctx);
CONST_STRING ZP_DestroyContextFuncName = "DestroyContext";
ZP_EXPORT ZP_Result zpDestroyContext(ZP_Context ctx);

typedef ZP_Result (*ZP_SetContextDataFunc)(ZP_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t ctxDataSizeBytes);
CONST_STRING ZP_SetContextDataFuncName = "SetContextData";
ZP_EXPORT ZP_Result zpSetContextData(ZP_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t ctxDataSizeBytes);

typedef ZP_Result (*ZP_UnsetContextDataFunc)(ZP_Context ctx, char* name, size_t nameSizeBytes);
CONST_STRING ZP_UnsetContextDataFuncName = "UnsetContextData";
ZP_EXPORT ZP_Result zpUnsetContextData(ZP_Context ctx, char* name, size_t nameSizeBytes);

struct ZP_ContextEnumeratorHandle_t;
typedef struct ZP_ContextEnumeratorHandle_t* ZP_ContextEnumeratorHandle;

typedef ZP_Result (*ZP_BeginEnumerateContextFunc)(ZP_Context ctx, ZP_ContextEnumeratorHandle* ctxEnum);
CONST_STRING ZP_BeginEnumerateContextFuncName = "BeginEnumerateContext";
ZP_EXPORT ZP_Result zpBeginEnumerateContext(ZP_Context ctx, ZP_ContextEnumeratorHandle* ctxEnum);

typedef ZP_Result (*ZP_EnumerateContextFunc)(ZP_ContextEnumeratorHandle ctxEnum, char** name, size_t* nameSizeBytes, void** data, size_t* dataSizeBytes);
CONST_STRING ZP_EnumerateContextFuncName = "EnumerateContext";
ZP_EXPORT ZP_Result zpEnumerateContext(ZP_ContextEnumeratorHandle ctxEnum, char** name, size_t* nameSizeBytes, void** data, size_t* dataSizeBytes);

typedef ZP_Result (*ZP_EndEnumerateContextFunc)(ZP_ContextEnumeratorHandle ctxEnum);
CONST_STRING ZP_EndEnumerateContextFuncName = "EndEnumerateContext";
ZP_EXPORT ZP_Result zpEndEnumerateContext(ZP_ContextEnumeratorHandle ctxEnum);

#pragma endregion Context API

#pragma region Portal API

struct ZP_Portal_t;
typedef struct ZP_Portal_t* ZP_Portal;

struct ZP_PortalParams_t
{
  char* id;
  size_t idSizeBytes;
};
typedef struct ZP_PortalParams_t* ZP_PortalParams;

typedef ZP_Result (*ZP_CreatePortalFunc)(ZP_Module zpm, ZP_PortalParams params, ZP_Portal* portal);
CONST_STRING ZP_CreatePortalFuncName = "CreatePortal";
ZP_EXPORT ZP_Result zpCreatePortal(ZP_Module zpm, ZP_PortalParams params, ZP_Portal* portal);

typedef ZP_Result (*ZP_ReleasePortalFunc)(ZP_Portal portal);
CONST_STRING ZP_ReleasePortalFuncName = "ReleasePortal";
ZP_EXPORT ZP_Result zpReleasePortal(ZP_Portal portal);

struct ZP_Node_t;
typedef struct ZP_Node_t* ZP_Node;

#pragma endregion Portal API

#pragma region Node API

typedef ZP_Result (*ZP_CreateNodeFunc)(ZP_Portal portal, const char* id, size_t idSizeBytes, ZP_Node* node);
CONST_STRING ZP_CreateNodeFuncName = "CreateNode";
ZP_EXPORT ZP_Result zpCreateNode(ZP_Portal portal, const char* id, size_t idSizeBytes, ZP_Node* node);

typedef ZP_Result (*ZP_AcquireNodeFunc)(ZP_Portal portal, ZP_Node node, ZP_Context context, ZP_Node* transformed);
CONST_STRING ZP_AcquireNodeFuncName = "AcquireNode";
ZP_EXPORT ZP_Result zpAcquireNode(ZP_Portal portal, ZP_Node node, ZP_Context context, ZP_Node* transformed);

typedef ZP_Result (*ZP_ReleaseNodeFunc)(ZP_Node node);
CONST_STRING ZP_ReleaseNodeFuncName = "ReleaseNode";
ZP_EXPORT ZP_Result zpReleaseNode(ZP_Node node);

#pragma endregion Node API

#pragma region Event API

struct ZP_Event_t;
typedef struct ZP_Event_t* ZP_Event;

#pragma endregion Event API

#pragma region CommandQueue API

enum ZP_CommandQueueFlags_t {
  ZP_QUEUE_DEFAULT = 0,
  ZP_QUEUE_OUT_OF_ORDER_ENABLE = 1 >> 1,
  ZP_QUEUE_PROFILING_ENABLE = 1 >> 2
};
typedef ZP_CommandQueueFlags_t ZP_CommandQueueFlags;

struct ZP_CommandQueueProperties_t {
  ZP_CommandQueueFlags flags;
  int32_t max_tries;
};
typedef ZP_CommandQueueProperties_t ZP_CommandQueueProperties;

struct ZP_CommandQueue_t;
typedef struct ZP_CommandQueue_t* ZP_CommandQueue;

typedef ZP_Result (*ZP_CreateCommandQueueFunc)(ZP_Portal portal, ZP_CommandQueueProperties properties, ZP_CommandQueue* commandQueue);
CONST_STRING ZP_CreateCommandQueueFuncName = "CreateCommandQueue";
ZP_EXPORT ZP_Result zpCreateCommandQueue(ZP_Portal portal, ZP_CommandQueueProperties properties, ZP_CommandQueue* commandQueue);

typedef ZP_Result (*ZP_ReleaseCommandQueueFunc)(ZP_CommandQueue commandQueue);
CONST_STRING ZP_ReleaseCommandQueueFuncName = "ReleaseCommandQueue";
ZP_EXPORT ZP_Result zpReleaseCommandQueue(ZP_CommandQueue commandQueue);

#pragma endregion CommandQueue API

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
typedef struct ZP_MemParams_t* ZP_MemParams;

typedef ZP_Result (*ZP_CreateMemFunc)(ZP_Portal portal, ZP_MemParams params, void* data, ZP_Mem* mem);
CONST_STRING ZP_CreateMemFuncName = "CreateMem";
ZP_EXPORT ZP_Result zpCreateMem(ZP_Portal portal, ZP_MemParams params, void* data, ZP_Mem* mem);

typedef ZP_Result (*ZP_ReleaseMemFunc)(ZP_Mem mem);
CONST_STRING ZP_ReleaseMemFuncName = "ReleaseMem";
ZP_EXPORT ZP_Result zpReleaseMem(ZP_Mem mem);

#pragma endregion Memory API

#pragma region Execution API

typedef ZP_Result (*ZP_EnqueueWriteMemFunc)(ZP_CommandQueue commandQueue, ZP_Mem mem, bool blocking_write, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);
CONST_STRING ZP_EnqueueWriteMemFuncName = "EnqueueWriteMem";
ZP_EXPORT ZP_Result zpEnqueueWriteMem(ZP_CommandQueue commandQueue, ZP_Mem mem, bool blocking_write, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);

typedef ZP_Result (*ZP_EnqueueReadMemFunc)(ZP_CommandQueue commandQueue, ZP_Mem mem, bool blocking_read, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);
CONST_STRING ZP_EnqueueReadMemFuncName = "EnqueueReadMem";
ZP_EXPORT ZP_Result zpEnqueueReadMem(ZP_CommandQueue commandQueue, ZP_Mem mem, bool blocking_read, size_t byte_offset, int64_t* strides, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);

typedef ZP_Result (*ZP_EnqueueStepFunc)(ZP_CommandQueue commandQueue, ZP_Node node, ZP_Context context, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);
CONST_STRING ZP_EnqueueStepFuncName = "EnqueueStep";
ZP_EXPORT ZP_Result zpEnqueueStep(ZP_CommandQueue commandQueue, ZP_Node node, ZP_Context context, uint32_t num_events_in_wait_list, ZP_Event* event_wait_list, ZP_Event* event);

typedef ZP_Result (*ZP_FlushFunc)(ZP_CommandQueue commandQueue);
CONST_STRING ZP_FlushFuncName = "Flush";
ZP_EXPORT ZP_Result zpFlush(ZP_CommandQueue commandQueue);

typedef ZP_Result (*ZP_FinishFunc)(ZP_CommandQueue commandQueue);
CONST_STRING ZP_FinishFuncName = "Finish";
ZP_EXPORT ZP_Result zpFinish(ZP_CommandQueue commandQueue);

#pragma endregion Execution API