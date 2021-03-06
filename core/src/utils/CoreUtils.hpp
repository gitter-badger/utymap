#ifndef UTILS_COREUTILS_HPP_DEFINED
#define UTILS_COREUTILS_HPP_DEFINED

#include <chrono>
#include <string>
#include <sstream>

namespace utymap { namespace utils {

template <typename T>
inline std::string toString(T t) 
{
    std::stringstream stream;
    stream << t;
    return stream.str();
}

inline bool endsWith(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) 
        return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F&& func, Args&&... args)
    {
        auto start = std::chrono::system_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast< TimeT>
                            (std::chrono::system_clock::now() - start);
        return duration.count();
    }
};

}}

#endif // UTILS_COREUTILS_HPP_DEFINED
