#pragma once
#include <string_view>

constexpr auto _auby_getOrig(std::string_view fn_n) {
    int n = fn_n.substr(10)[0];
    if (n == '0')
        return 0;
    if (n == '1')
        return 1;
    if (n == '2')
        return 2;
    if (n == '3')
        return 3;
    if (n == '4')
        return 4;
    if (n == '5')
        return 5;
    if (n == '6')
        return 6;
    if (n == '7')
        return 7;
    if (n == '8')
        return 8;
    if (n == '9')
        return 9;
    return -1;
}

#define AUBY_CONCAT_(x, y) x##y
#define AUBY_CONCAT(x, y) AUBY_CONCAT_(x, y)

#define AUBY_MODIFY_IMPL0(...) AUBY_MODIFY_IMPL(0, __VA_ARGS__)
#define AUBY_MODIFY_IMPL1(...) AUBY_MODIFY_IMPL(1, __VA_ARGS__)
#define AUBY_MODIFY_IMPL2(...) AUBY_MODIFY_IMPL(2, __VA_ARGS__)
#define AUBY_MODIFY_IMPL3(...) AUBY_MODIFY_IMPL(3, __VA_ARGS__)
#define AUBY_MODIFY_IMPL4(...) AUBY_MODIFY_IMPL(4, __VA_ARGS__)
#define AUBY_MODIFY_IMPL5(...) AUBY_MODIFY_IMPL(5, __VA_ARGS__)
#define AUBY_MODIFY_IMPL6(...) AUBY_MODIFY_IMPL(6, __VA_ARGS__)
#define AUBY_MODIFY_IMPL7(...) AUBY_MODIFY_IMPL(7, __VA_ARGS__)
#define AUBY_MODIFY_IMPL8(...) AUBY_MODIFY_IMPL(8, __VA_ARGS__)
#define AUBY_MODIFY_IMPL9(...) AUBY_MODIFY_IMPL(9, __VA_ARGS__)

#define AUBY_MODIFY_IMPL(COUNTER, ...)                                         \
    decltype(AUBY_CONCAT(_auby_retTy, COUNTER)                                 \
    ) AUBY_CONCAT(_auby_hook, COUNTER)(__VA_ARGS__);                           \
    [[gnu::constructor]] void AUBY_CONCAT(_auby_registerHook, COUNTER)() {     \
        auby::hook<&AUBY_CONCAT(_auby_hook, COUNTER)>(                         \
            AUBY_CONCAT(_auby_target, COUNTER)                                 \
        );                                                                     \
    }                                                                          \
    template <int>                                                             \
    struct _auby_orig;                                                         \
    template <>                                                                \
    struct _auby_orig<COUNTER> {                                               \
        template <class... Args>                                               \
        static inline auto call(Args... args) {                                \
            return auby::orig<&AUBY_CONCAT(_auby_hook, COUNTER)>(args...);     \
        }                                                                      \
    };                                                                         \
    decltype(AUBY_CONCAT(_auby_retTy, COUNTER))                                \
    AUBY_CONCAT(_auby_hook, COUNTER)(__VA_ARGS__)

#define AUBY_MODIFY(COUNTER, FN)                                               \
    AUBY_CONCAT(_auby_retTy, COUNTER);                                         \
    constexpr auto AUBY_CONCAT(_auby_target, COUNTER) = #FN;                   \
    AUBY_CONCAT(AUBY_MODIFY_IMPL, COUNTER)

#define $(FN) AUBY_MODIFY(__COUNTER__, FN)

#define $orig(...) _auby_orig<_auby_getOrig(__FUNCTION__)>::call(__VA_ARGS__)
