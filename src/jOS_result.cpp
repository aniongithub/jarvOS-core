#include <jarvOS/jOS.h>

#include <limits>
#include <string.h>

#include <magic_enum.hpp>

jOS_Result jOSGetResultString(jOS_Result result, char* buf, size_t* bufSizeBytes)
{
    if (!bufSizeBytes)
        return jOS_Result::jOS_RESULT_INVALID_ARGUMENTS;

    auto value = magic_enum::enum_name(result);
    if (buf)
        strncpy(buf, &value[0], std::min(value.length(), *bufSizeBytes));
    else
        *bufSizeBytes = value.length();

    return jOS_Result::jOS_RESULT_OK;
}