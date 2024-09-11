#include <algorithm>
#include "ViciClient/include/Comparators/CaseInsensitiveComparator.hpp"

bool CaseInsensitiveComparator::operator()(const std::string& lhs, const std::string& rhs) const {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
        [](char a, char b) {
            return std::tolower(a) < std::tolower(b);
        });
}