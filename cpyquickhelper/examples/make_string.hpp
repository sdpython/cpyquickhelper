#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <sstream>
#include <vector>
#include <thread>
#include <iterator>
#include <iostream>

template <typename T>
inline void MakeStringInternal(std::ostringstream& ss, const T& t) noexcept {
    ss << t;
}

template <>
inline void MakeStringInternal(std::ostringstream& ss, const std::vector<char>& t) noexcept {
    for (auto it : t) {
        ss << it << ",";
    }
}

template <>
inline void MakeStringInternal(std::ostringstream& ss, const std::vector<int64_t>& t) noexcept {
    for (auto it : t) {
        ss << it << ",";
    }
}

template <>
inline void MakeStringInternal(std::ostringstream& ss, const std::vector<size_t>& t) noexcept {
    for (auto it : t) {
        ss << it << ",";
    }
}

template <>
inline void MakeStringInternal(std::ostringstream& ss, const std::pair<int64_t, size_t>& t) noexcept {
    ss << "(" << t.first << "," << t.second << ")";
}

template <typename T, typename... Args>
inline void MakeStringInternal(std::ostringstream& ss, const T& t, const Args&... args) noexcept {
    MakeStringInternal(ss, t);
    MakeStringInternal(ss, args...);
}

template <typename... Args>
inline std::string MakeString(const Args&... args) {
    std::ostringstream ss;
    MakeStringInternal(ss, args...);
    return std::string(ss.str());
}
