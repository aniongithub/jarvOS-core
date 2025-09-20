#pragma once

#include <polyverse/pv.h>

struct pv_Module_t {
    private:
        void* _moduleHdl;

        pv_InitModuleFunc _initModuleFunc;
        pv_shutdownModuleFunc _shutdownModuleFunc;
    public:
        pv_Module_t();
        pv_Result load(pv_ModuleParams params);
        ~pv_Module_t();
};