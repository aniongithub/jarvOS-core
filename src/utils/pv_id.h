#pragma once

#include <polyverse/pv.h>

#include <fmt/core.h>
#include <string>

static constexpr std::string_view null_ModuleId("null");

#define MAKE_ID(params) make_string_view(params.id, params.idSizeBytes)
const std::string_view make_string_view(const char* id, const size_t sizeBytes);
const bool compareIds(const char* id_a, size_t id_a_sizeBytes, const char* id_b, size_t id_b_sizeBytes);
const bool compareIds(const std::string_view& id_a, const std::string_view& id_b);
std::string normalizeId(const std::string_view id);