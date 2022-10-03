#pragma once

#include <zeropoint/zp.h>

struct ZP_Lib_t {
    private:
        ZP_CreateContextFunc createContextFunc;
        ZP_DestroyContextFunc destroyContextFunc;
    public:
        ZP_Lib_t();
        ~ZP_Lib_t();
};