#pragma once

#include <jarvOS/jOS.h>

struct jOS_Module_t {
    private:
        void* moduleHdl;

        jOS_InitModuleFunc initModule;
        jOS_shutdownModuleFunc shutdownModule;
    public:
        jOS_Module_t();
        jOS_Result load(jOS_ModuleParams params);
        ~jOS_Module_t();
};