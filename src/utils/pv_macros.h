#pragma once

#include <string>
#include <dlfcn.h>

#include <fmt/core.h>

#include <polyverse/pv.h>
#include <utils/pv_id.h>
#include <pv_log.h>

#define MAKE_ID(params) make_string_view(params.id, params.idSizeBytes)
#define STRING_VIEW(obj, name) make_string_view(obj.name, obj.name##SizeBytes)

template <typename T, typename ... Args>
pv_Result getFunc(void* hdl, T& func, const std::string_view& formatStr, Args ... args)
{
    auto name = fmt::format(formatStr, args ...);
    func = (const T)dlsym(hdl, name.c_str());
    if (func)
    {
        PV_LOGDEBUG("API impl {} loaded", name.c_str());
        return pv_Result::PV_RESULT_OK;
    }
    else
    {
        PV_LOGWARNING("API impl {} not found", name.c_str());
        return pv_Result::PV_RESULT_NOT_SUPPORTED;
    }
}
#define GET_FUNC_PTR(funcName, libHdl, id, funcPtr) getFunc<funcName>(libHdl, funcPtr, "{}_{}", id, funcName ## Name)