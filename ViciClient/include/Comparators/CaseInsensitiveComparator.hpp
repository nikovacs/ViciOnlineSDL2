#pragma once

#include <string>

struct CaseInsensitiveComparator {
    bool operator()(const std::string& lhs, const std::string& rhs) const;
};