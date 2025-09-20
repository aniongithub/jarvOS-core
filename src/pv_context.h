#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <cstring>

#include <polyverse/pv.h>

// Helper struct to store data with its size
struct ContextData {
    std::unique_ptr<uint8_t[]> data;
    size_t size;
    
    ContextData() : data(nullptr), size(0) {}
    ContextData(const void* srcData, size_t srcSize) : size(srcSize) {
        if (srcData && srcSize > 0) {
            data = std::make_unique<uint8_t[]>(srcSize);
            std::memcpy(data.get(), srcData, srcSize);
        }
    }
    
    // Move constructor
    ContextData(ContextData&& other) noexcept 
        : data(std::move(other.data)), size(other.size) {
        other.size = 0;
    }
    
    // Move assignment
    ContextData& operator=(ContextData&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            size = other.size;
            other.size = 0;
        }
        return *this;
    }
    
    // Delete copy constructor and assignment to avoid accidental copies
    ContextData(const ContextData&) = delete;
    ContextData& operator=(const ContextData&) = delete;
};

struct pv_Context_t {
    private:
        std::map<std::string, ContextData> _contextMap;
        
    public:
        pv_Context_t();
        ~pv_Context_t();

        pv_Result setData(const std::string& name, const void* ctxData, size_t ctxDataSizeBytes);
        pv_Result unsetData(const std::string& name);
        pv_Result getData(const std::string& name, void** data, size_t* size);
        bool hasData(const std::string& name) const;
        std::vector<std::string> getKeys() const;
        void clear();
        
        // Friend class and function for iterator access
        friend class pv_ContextEnumeratorHandle_t;
        friend pv_Result pvEnumerateContext(pv_ContextEnumeratorHandle ctxEnum, char** name, size_t* nameSizeBytes, void** data, size_t* dataSizeBytes);
};

struct pv_ContextEnumeratorHandle_t {
    public:
        pv_Context _context;
        std::map<std::string, ContextData>::iterator _current;
        
        pv_ContextEnumeratorHandle_t(pv_Context ctx);
        ~pv_ContextEnumeratorHandle_t();
};