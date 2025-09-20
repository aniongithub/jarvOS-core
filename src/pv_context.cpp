#include <pv_context.h>
#include <pv_log.h>
#include <utils/pv_id.h>

// pv_Context_t implementation
pv_Context_t::pv_Context_t()
{
    PV_LOGDEBUG("Created context {}", static_cast<void*>(this));
}

pv_Context_t::~pv_Context_t()
{
    clear();
    PV_LOGDEBUG("Destroyed context {}", static_cast<void*>(this));
}

pv_Result pv_Context_t::setData(const std::string& name, const void* ctxData, size_t ctxDataSizeBytes)
{
    if (name.empty()) {
        PV_LOGERROR("Invalid arguments: name is empty");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    if (!ctxData && ctxDataSizeBytes > 0) {
        PV_LOGERROR("Invalid arguments: ctxData is null but size is non-zero");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    try {
        _contextMap[name] = ContextData(ctxData, ctxDataSizeBytes);
        PV_LOGDEBUG("Set context data for key '{}' with {} bytes", name, ctxDataSizeBytes);
        return PV_RESULT_OK;
    } catch (const std::exception& e) {
        PV_LOGERROR("Failed to set context data for key '{}': {}", name, e.what());
        return PV_RESULT_CONTEXT_ERROR;
    }
}

pv_Result pv_Context_t::unsetData(const std::string& name)
{
    if (name.empty()) {
        PV_LOGERROR("Invalid arguments: name is empty");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    auto it = _contextMap.find(name);
    if (it == _contextMap.end()) {
        PV_LOGWARNING("Context data for key '{}' not found", name);
        return PV_RESULT_NOT_SUPPORTED;
    }
    
    _contextMap.erase(it);
    PV_LOGDEBUG("Unset context data for key '{}'", name);
    return PV_RESULT_OK;
}

pv_Result pv_Context_t::getData(const std::string& name, void** data, size_t* size)
{
    if (name.empty() || !data || !size) {
        PV_LOGERROR("Invalid arguments");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    auto it = _contextMap.find(name);
    if (it == _contextMap.end()) {
        PV_LOGDEBUG("Context data for key '{}' not found", name);
        *data = nullptr;
        *size = 0;
        return PV_RESULT_NOT_SUPPORTED;
    }
    
    *data = it->second.data.get();
    *size = it->second.size;
    PV_LOGDEBUG("Retrieved context data for key '{}' with {} bytes", name, *size);
    return PV_RESULT_OK;
}

bool pv_Context_t::hasData(const std::string& name) const
{
    return _contextMap.find(name) != _contextMap.end();
}

std::vector<std::string> pv_Context_t::getKeys() const
{
    std::vector<std::string> keys;
    keys.reserve(_contextMap.size());
    
    for (const auto& pair : _contextMap) {
        keys.push_back(pair.first);
    }
    
    return keys;
}

void pv_Context_t::clear()
{
    _contextMap.clear();
    PV_LOGDEBUG("Cleared all context data");
}

// pv_ContextEnumeratorHandle_t implementation
pv_ContextEnumeratorHandle_t::pv_ContextEnumeratorHandle_t(pv_Context ctx)
    : _context(ctx)
{
    if (_context) {
        _current = _context->_contextMap.begin();
        PV_LOGDEBUG("Created context enumerator with {} keys", _context->_contextMap.size());
    } else {
        PV_LOGERROR("Invalid context provided to enumerator");
    }
}

pv_ContextEnumeratorHandle_t::~pv_ContextEnumeratorHandle_t()
{
    PV_LOGDEBUG("Destroyed context enumerator");
}

// C API implementations
pv_Result pvCreateContext(pv_Context* ctx)
{
    if (!ctx) {
        PV_LOGERROR("Invalid arguments: ctx is null");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    try {
        *ctx = new pv_Context_t();
        PV_LOGDEBUG("Created context {}", static_cast<void*>(*ctx));
        return PV_RESULT_OK;
    } catch (const std::exception& e) {
        PV_LOGERROR("Failed to create context: {}", e.what());
        return PV_RESULT_CONTEXT_ERROR;
    }
}

pv_Result pvDestroyContext(pv_Context ctx)
{
    if (!ctx) {
        PV_LOGERROR("Invalid arguments: ctx is null");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    PV_LOGDEBUG("Destroying context {}", static_cast<void*>(ctx));
    delete ctx;
    return PV_RESULT_OK;
}

pv_Result pvGetContextData(pv_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t* ctxDataSizeBytes)
{
    if (!ctx || !name || nameSizeBytes == 0 || !ctxDataSizeBytes) {
        PV_LOGERROR("Invalid arguments");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    std::string nameStr(make_string_view(name, nameSizeBytes));
    void* data = nullptr;
    size_t size = 0;
    
    pv_Result result = ctx->getData(nameStr, &data, &size);
    if (result != PV_RESULT_OK) {
        *ctxDataSizeBytes = 0;
        return result;
    }
    
    // If ctxData is NULL, user is just querying the size
    if (!ctxData) {
        *ctxDataSizeBytes = size;
        return PV_RESULT_OK;
    }
    
    // If buffer is too small, return error and required size
    if (*ctxDataSizeBytes < size) {
        *ctxDataSizeBytes = size;
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    // Copy data to user's buffer (safe)
    memcpy(ctxData, data, size);
    *ctxDataSizeBytes = size;
    return PV_RESULT_OK;
}

pv_Result pvSetContextData(pv_Context ctx, char* name, size_t nameSizeBytes, void* ctxData, size_t ctxDataSizeBytes)
{
    if (!ctx || !name || nameSizeBytes == 0) {
        PV_LOGERROR("Invalid arguments");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    std::string nameStr(make_string_view(name, nameSizeBytes));
    return ctx->setData(nameStr, ctxData, ctxDataSizeBytes);
}

pv_Result pvUnsetContextData(pv_Context ctx, char* name, size_t nameSizeBytes)
{
    if (!ctx || !name || nameSizeBytes == 0) {
        PV_LOGERROR("Invalid arguments");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    std::string nameStr(make_string_view(name, nameSizeBytes));
    return ctx->unsetData(nameStr);
}

pv_Result pvBeginEnumerateContext(pv_Context ctx, pv_ContextEnumeratorHandle* ctxEnum)
{
    if (!ctx || !ctxEnum) {
        PV_LOGERROR("Invalid arguments");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    try {
        *ctxEnum = new pv_ContextEnumeratorHandle_t(ctx);
        PV_LOGDEBUG("Created context enumerator {}", static_cast<void*>(*ctxEnum));
        return PV_RESULT_OK;
    } catch (const std::exception& e) {
        PV_LOGERROR("Failed to create context enumerator: {}", e.what());
        return PV_RESULT_CONTEXT_ERROR;
    }
}

pv_Result pvEnumerateContext(pv_ContextEnumeratorHandle ctxEnum, char** name, size_t* nameSizeBytes, void** data, size_t* dataSizeBytes)
{
    if (!ctxEnum || !name || !nameSizeBytes || !data || !dataSizeBytes) {
        PV_LOGERROR("Invalid arguments");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    // Check if context is invalid
    if (!ctxEnum->_context) {
        PV_LOGERROR("Invalid context in enumerator");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    // Check if we've reached the end (completion state)
    if (ctxEnum->_current == ctxEnum->_context->_contextMap.end()) {
        PV_LOGDEBUG("Enumerator has reached the end");
        *name = nullptr;
        *nameSizeBytes = 0;
        *data = nullptr;
        *dataSizeBytes = 0;
        return PV_COMPLETE;
    }
    
    // Access current iterator element directly
    const std::string& currentKey = ctxEnum->_current->first;
    const ContextData& currentData = ctxEnum->_current->second;
    
    // Return pointers to internal data
    *name = const_cast<char*>(currentKey.c_str());
    *nameSizeBytes = currentKey.length();
    *data = currentData.data.get();
    *dataSizeBytes = currentData.size;
    
    PV_LOGDEBUG("Enumerated context key '{}' with {} bytes", currentKey, currentData.size);
    
    // Move to next item
    ++ctxEnum->_current;
    return PV_RESULT_OK;
}

pv_Result pvEndEnumerateContext(pv_ContextEnumeratorHandle ctxEnum)
{
    if (!ctxEnum) {
        PV_LOGERROR("Invalid arguments: ctxEnum is null");
        return PV_RESULT_INVALID_ARGUMENTS;
    }
    
    PV_LOGDEBUG("Destroying context enumerator {}", static_cast<void*>(ctxEnum));
    delete ctxEnum;
    return PV_RESULT_OK;
}