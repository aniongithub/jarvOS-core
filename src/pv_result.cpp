#include <polyverse/pv.h>

#include <limits>
#include <string.h>

#include <magic_enum/magic_enum.hpp>

pv_Result pvGetResultString(pv_Result result, char* buf, size_t* bufSizeBytes)
{
    if (!bufSizeBytes)
        return pv_Result::pv_RESULT_INVALID_ARGUMENTS;

    auto value = magic_enum::enum_name(result);
    if (buf)
        strncpy(buf, &value[0], std::min(value.length(), *bufSizeBytes));
    else
        *bufSizeBytes = value.length();

    return pv_Result::pv_RESULT_OK;
}