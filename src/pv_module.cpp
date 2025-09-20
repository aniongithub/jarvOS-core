#include <pv_module.h>
#include <pv_log.h>

#include <utils/pv_macros.h>
#include <fmt/format.h>

#include <dlfcn.h>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

#include <limits.h>

// TODO: Use something like flexdll when porting to/testing on Windows
// https://github.com/ocaml/flexdll

pv_Module_t::pv_Module_t()
{
}

pv_Result pv_Module_t::load(pv_ModuleParams params)
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
        PV_LOGVERBOSE("Going to try and acquire module from {}", libName.c_str());
    }

    hdl = dlopen(strlen(module_path) == 0 ? nullptr : module_path, RTLD_GLOBAL | RTLD_LAZY);
    if (!hdl)
    {
        PV_LOGERROR("Could not load module at {}", module_path);
        return pv_Result::pv_RESULT_MODULE_LOAD_ERROR;
    }
    else
        PV_LOGDEBUG("Successfully loaded module with dl handle {}", hdl);

    auto id = (params.id && (params.idSizeBytes > 0)) ?
        STRING_VIEW(params, id) :
        std::string_view("");
    PV_LOGVERBOSE("Loading function pointers for id: \"{}\"", id);
    GET_FUNC_PTR(pv_InitModuleFunc, _moduleHdl, id, _initModuleFunc);
    GET_FUNC_PTR(pv_shutdownModuleFunc, _moduleHdl, id, _shutdownModuleFunc);

    // Save this
    _moduleHdl = hdl;
    PV_LOGVERBOSE("Loaded module with dl handle {}", hdl);
    return pv_RESULT_OK;
}

pv_Module_t::~pv_Module_t()
{
    if (_moduleHdl)
    {
        auto result = dlclose(_moduleHdl);
        if (result)
            PV_LOGDEBUG("dlclose error {} shutting down module with handle {}", result, _moduleHdl);
        else
            PV_LOGDEBUG("Successfully shut down module");
    }
}

pv_Result pvAcquireModule(pv_ModuleParams params, pv_Module* pvmodule)
{
    if (!pvmodule)
    {
        PV_LOGERROR("Invalid arguments: pvModule is empty");
        return pv_Result::pv_RESULT_INVALID_ARGUMENTS;
    }

    auto moduleHdl = new pv_Module_t();
    auto result = moduleHdl->load(params);
    if (result != pv_Result::pv_RESULT_OK)
    {
        delete moduleHdl;
        return result;
    }

    *pvmodule = moduleHdl;
    PV_LOGDEBUG("Acquired module with handle {}", static_cast<void*>(moduleHdl));
    return pv_Result::pv_RESULT_OK;
}

pv_Result pvReleaseModule(pv_Module pvmodule)
{
    if (!pvmodule)
    {
        PV_LOGERROR("Invalid arguments: No module specified");
        return pv_Result::pv_RESULT_INVALID_ARGUMENTS;
    }

    delete pvmodule;
    PV_LOGDEBUG("Released module with handle {}", static_cast<void*>(pvmodule));
    return pv_Result::pv_RESULT_OK;
}