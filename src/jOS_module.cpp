#include <jOS_module.h>
#include <jOS_log.h>

#include <utils/jOS_macros.h>
#include <fmt/format.h>

#include <dlfcn.h>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

#include <limits.h>

// TODO: Use something like flexdll when porting to/testing on Windows
// https://github.com/ocaml/flexdll

jOS_Module_t::jOS_Module_t()
{
}

jOS_Result jOS_Module_t::load(jOS_ModuleParams params)
{
    void* hdl = nullptr;
    fs::path hintPath = (params.hintPath && (params.hintPathSizeBytes > 0)) ?
        fs::path(STRING_VIEW(params, hintPath)) :
        fs::path("");

    char module_path[PATH_MAX] = {0};
    if (params.name && (params.nameSizeBytes > 0))
    {
        fs::path libName(STRING_VIEW(params, name));
        libName = hintPath / libName;
        strncpy(module_path, libName.c_str(), strlen(libName.c_str()));
        JOS_LOGVERBOSE("Going to try and acquire module from {}", libName.c_str());
    }

    hdl = dlopen(strlen(module_path) == 0 ? nullptr : module_path, RTLD_GLOBAL | RTLD_LAZY);
    if (!hdl)
    {
        JOS_LOGERROR("Could not load module at {}", module_path);
        return jOS_Result::jOS_RESULT_MODULE_LOAD_ERROR;
    }
    else
        JOS_LOGDEBUG("Successfully loaded module with dl handle {}", hdl);

    auto id = (params.id && (params.idSizeBytes > 0)) ?
        STRING_VIEW(params, id) :
        std::string_view("");
    JOS_LOGVERBOSE("Loading function pointers for id: \"{}\"", id);
    GET_FUNC_PTR(jOS_InitModuleFunc, _moduleHdl, id, _initModuleFunc);
    GET_FUNC_PTR(jOS_shutdownModuleFunc, _moduleHdl, id, _shutdownModuleFunc);

    // Save this
    _moduleHdl = hdl;
    JOS_LOGVERBOSE("Loaded module with dl handle {}", hdl);
    return jOS_RESULT_OK;
}

jOS_Module_t::~jOS_Module_t()
{
    if (_moduleHdl)
    {
        auto result = dlclose(_moduleHdl);
        if (result)
            JOS_LOGDEBUG("dlclose error {} shutting down module with handle {}", result, _moduleHdl);
        else
            JOS_LOGDEBUG("Successfully shut down module");
    }
}

jOS_Result jOSAcquireModule(jOS_ModuleParams params, jOS_Module* jOSmodule)
{
    if (!jOSmodule)
    {
        JOS_LOGERROR("Invalid arguments: jOSModule is empty");
        return jOS_Result::jOS_RESULT_INVALID_ARGUMENTS;
    }

    auto moduleHdl = new jOS_Module_t();
    auto result = moduleHdl->load(params);
    if (result != jOS_Result::jOS_RESULT_OK)
    {
        delete moduleHdl;
        return result;
    }

    *jOSmodule = moduleHdl;
    JOS_LOGDEBUG("Acquired module with handle {}", static_cast<void*>(moduleHdl));
    return jOS_Result::jOS_RESULT_OK;
}

jOS_Result jOSReleaseModule(jOS_Module jOSmodule)
{
    if (!jOSmodule)
    {
        JOS_LOGERROR("Invalid arguments: No module specified");
        return jOS_Result::jOS_RESULT_INVALID_ARGUMENTS;
    }

    delete jOSmodule;
    JOS_LOGDEBUG("Released module with handle {}", static_cast<void*>(jOSmodule));
    return jOS_Result::jOS_RESULT_OK;
}