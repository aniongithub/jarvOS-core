#pragma once

#include <zeropoint/zp.h>
#include <fmt/core.h>
#include <string>
#include <dlfcn.h>

#include <utils/zp_id.h>

#define MAKE_ID(params) make_string_view(params.id, params.idSizeBytes)
#define STRING_VIEW(obj, name) make_string_view(obj.name, obj.name##SizeBytes)

template <typename T, typename ... Args>
ZP_Result getFunc(void* hdl, T& func, const std::string_view& formatStr, Args ... args)
{
    auto name = fmt::format(formatStr, args ...);
    func = (const T)dlsym(hdl, name.c_str());
    return func ? ZP_RESULT_OK : ZP_RESULT_NOT_SUPPORTED;
}
#define GET_FUNC_PTR(funcName, libHdl, id, funcPtr) getFunc<funcName>(libHdl, funcPtr, "{}_{}", id, funcName ## Name)