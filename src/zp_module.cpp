#include <zp_module.h>
#include <utils/zp_macros.h>

#include <dlfcn.h>
#include <string>

ZP_Module_t::ZP_Module_t()
{
}

ZP_Result ZP_Module_t::load(ZP_ModuleParams params)
{
    void* hdl = nullptr;
    if (params.id && (params.idSizeBytes > 0)) // Load shared lib
    {
        std::string libName(params.id, params.idSizeBytes);
        if (params.hintPath && (params.hintPathSizeBytes > 0))
        {
            std::string hintPath(params.hintPath, params.hintPathSizeBytes);
            libName = hintPath + "/" + libName;
        }
        libName += ".so";
        hdl = dlopen(libName.c_str(), RTLD_GLOBAL | RTLD_LAZY);
    }
    else
        hdl = dlopen(nullptr, RTLD_GLOBAL | RTLD_LAZY); // Built-in

    if (!hdl)
        return ZP_RESULT_MODULE_NOT_FOUND;

    auto result = GET_FUNC_PTR(ZP_InitModuleFunc, hdl, MAKE_ID(params), initModule);

    return ZP_RESULT_NOT_IMPLEMENTED;    
}

ZP_Module_t::~ZP_Module_t()
{
}

ZP_Result zpAcquireModule(ZP_ModuleParams params, ZP_Module* zpm)
{
    return ZP_RESULT_NOT_IMPLEMENTED;
}

ZP_Result zpReleaseModule(ZP_Module zpm)
{
    return ZP_RESULT_NOT_IMPLEMENTED;
}