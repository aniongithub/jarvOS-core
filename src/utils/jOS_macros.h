#pragma once

#include <string>
#include <dlfcn.h>

#include <fmt/core.h>

#include <jarvOS/jOS.h>
#include <utils/jOS_id.h>
#include <jOS_log.h>

#define MAKE_ID(params) make_string_view(params.id, params.idSizeBytes)
#define STRING_VIEW(obj, name) make_string_view(obj.name, obj.name##SizeBytes)

template <typename T, typename ... Args>
jOS_Result getFunc(void* hdl, T& func, const std::string_view& formatStr, Args ... args)
{
    auto name = fmt::format(formatStr, args ...);
    func = (const T)dlsym(hdl, name.c_str());
    if (func)
    {
        JOS_LOGDEBUG("API impl {} loaded", name.c_str());
        return jOS_Result::jOS_RESULT_OK;
    }
    else
    {
        JOS_LOGWARNING("API impl {} not found", name.c_str());
        return jOS_Result::jOS_RESULT_NOT_SUPPORTED;
    }
}
#define GET_FUNC_PTR(funcName, libHdl, id, funcPtr) getFunc<funcName>(libHdl, funcPtr, "{}_{}", id, funcName ## Name)