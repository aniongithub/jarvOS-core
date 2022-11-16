#pragma once

#include <jarvOS/jOS.h>

struct jOS_Module_t {
    private:
        void* _moduleHdl;

        jOS_InitModuleFunc _initModuleFunc;
        jOS_shutdownModuleFunc _shutdownModuleFunc;
    public:
        jOS_Module_t();
        jOS_Result load(jOS_ModuleParams params);
        ~jOS_Module_t();
};