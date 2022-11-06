#include <jOS_module.h>
#include <utils/jOS_macros.h>
#include <plog/Log.h>
#include <fmt/format.h>

#include <dlfcn.h>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

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
        return jOS_RESULT_INVALID_ARGUMENTS;
    }

    // Save this
    moduleHdl = hdl;

    // TODO: Load all function pointers here

    return moduleHdl ? jOS_RESULT_OK : jOS_RESULT_MODULE_NOT_FOUND;
}

jOS_Module_t::~jOS_Module_t()
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

jOS_Result jOSAcquireModule(jOS_ModuleParams params, jOS_Module* jOSm)
{
    return jOS_RESULT_NOT_IMPLEMENTED;
}

jOS_Result jOSReleaseModule(jOS_Module jOSm)
{
    return jOS_RESULT_NOT_IMPLEMENTED;
}