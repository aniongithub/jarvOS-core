#pragma once

#include <zeropoint/zp.h>

struct ZP_Module_t {
    private:
        void* moduleHdl;

        ZP_InitModuleFunc initModule;
        ZP_shutdownModuleFunc shutdownModule;
    public:
        ZP_Module_t();
        ZP_Result load(ZP_ModuleParams params);
        ~ZP_Module_t();
};