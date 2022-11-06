#include <utils/jOS_id.h>

const bool compareIds(const std::string_view& id_a, const std::string_view& id_b)
{
    return std::equal(
        id_a.begin(), id_a.end(), 
        id_b.begin(),
        [] (const char& a, const char& b)
        {
            // Case-insensitive, . and _ are interchangeable
            return (std::tolower(a) == std::tolower(b) || 
                ((a == '_') && (b == '.')) ||
                ((a == '.') && (b == '_'))
                );
        });
}

const std::string_view make_string_view(const char* id, const size_t sizeBytes)
{
    return std::string_view(id == nullptr ? null_ModuleId.data(): id, 
                       id == nullptr?  null_ModuleId.length(): sizeBytes);
}

const bool compareIds(const char* id_a, size_t id_a_sizeBytes, const char* id_b, size_t id_b_sizeBytes)
{
    return compareIds(make_string_view(id_a, id_a_sizeBytes), 
                      make_string_view(id_b, id_b_sizeBytes));
}

std::string normalizeId(const std::string_view id)
{
    std::string result(id);
    std::replace(result.begin(), result.end(), '.', '_');
    return result;
}