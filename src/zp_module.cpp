#include <zp_module.h>
#include <utils/zp_macros.h>
#include <plog/Log.h>
#include <fmt/format.h>

#include <dlfcn.h>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

// TODO: Use something like flexdll when porting to/testing on Windows
// https://github.com/ocaml/flexdll

ZP_Module_t::ZP_Module_t()
{
}

ZP_Result ZP_Module_t::load(ZP_ModuleParams params)
{
    void* hdl = nullptr;
    fs::path hintPath = (params.hintPath && (params.hintPathSizeBytes > 0)) ?
        fs::path(STRING_VIEW(params, hintPath)) :
        fs::path("");
    
    if (params.name && (params.nameSizeBytes > 0)) // Load shared lib
    {
        fs::path libName(STRING_VIEW(params, name));
        libName = hintPath / libName;
        PLOGV << fmt::format("Trying to load ZPM from {}", libName.c_str());
        hdl = dlopen(libName.c_str(), RTLD_GLOBAL | RTLD_LAZY);
        if (hdl)
            PLOGV << fmt::format("Successfully acquired ZPM at {}", libName.c_str());
        else
            PLOGV << fmt::format("Could not load ZPM at {}", libName.c_str());
    }
    else
    {
        PLOGD << "No id and/or hintPath specified";
        return ZP_RESULT_INVALID_ARGUMENTS;
    }

    // Save this
    moduleHdl = hdl;

    // TODO: Load all function pointers here

    return moduleHdl ? ZP_RESULT_OK : ZP_RESULT_MODULE_NOT_FOUND;
}

ZP_Module_t::~ZP_Module_t()
{
    if (moduleHdl)
    {
        auto result = dlclose(moduleHdl);
        if (result)
            PLOGD << fmt::format("Error {} shutting down ZPM with handle {}", result, moduleHdl);
        else
            PLOGD << "Successfully shut down ZPM";
    }
}

ZP_Result zpAcquireModule(ZP_ModuleParams params, ZP_Module* zpm)
{
    return ZP_RESULT_NOT_IMPLEMENTED;
}

ZP_Result zpReleaseModule(ZP_Module zpm)
{
    return ZP_RESULT_NOT_IMPLEMENTED;
}