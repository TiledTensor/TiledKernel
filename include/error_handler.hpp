#pragma once
#include <fmt/core.h>
#include <stdexcept>

namespace tiledkernel {
    struct UnimplementError : public std::logic_error {
        explicit UnimplementError(std::string msg)
            : std::logic_error(std::move(msg)) {}
    };

    struct UnreachableError : public std::logic_error {
        explicit UnreachableError(std::string msg)
            : std::logic_error(std::move(msg)) {}
    };
}  // namespace tiledkernel

#define ERROR_MSG(MSG) fmt::format("{} Source {}:{}", (MSG), __FILE__, __LINE__)
#define RUNTIME_ERROR(MSG) throw std::runtime_error(ERROR_MSG(MSG))
#define OUT_OF_RANGE(MSG, A, B) \
    throw std::out_of_range(ERROR_MSG(fmt::format("{}/{} {}", (A), (B), (MSG))))
#define TODO(MSG) throw tiledkernel::UnimplementError(ERROR_MSG(MSG))

#define UNREACHABLEX(T, F, ...)                                         \
    [&]() -> T {                                                        \
        throw tilegraph::UnreachableError(                              \
            ERROR_MSG(fmt::format("Unreachable: " #F, ##__VA_ARGS__))); \
    }()
#define UNREACHABLE() UNREACHABLEX(void, "no message")

#ifndef DISABLE_ASSERT
#define ASSERT(CONDITION, F, ...)                                        \
    {                                                                    \
        if (!(CONDITION))                                                \
            RUNTIME_ERROR(fmt::format("Assertion: " #F, ##__VA_ARGS__)); \
    }
#else
#define ASSERT(CONDITION, F)
#endif