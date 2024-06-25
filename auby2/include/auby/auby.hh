#pragma once

#include "internal.hh"
#include <cocos2d.h>
#include <gd/gd.hh>
#include <spdlog/spdlog.h>
#include <type_traits>

using namespace cocos2d;

namespace auby {
void alert(std::string msg);
void run_in_main_queue(std::function<void()> fn);

template <class T, class S>
T union_cast(S s) {
    union {
        T t;
        S s;
    } u;

    u.s = s;
    return u.t;
}

template <const auto fn>
void hook(std::string sig) {
    spdlog::info("Hooking {}", sig);
    internal::hook(internal::resolve(sig), union_cast<void*>(fn));
}

template <const auto fn, typename... Args>
static inline auto orig(Args... args) {
    auto orig_vp = internal::orig(union_cast<void*>(fn));
    auto orig_p = union_cast<decltype(fn)>(orig_vp);
    return orig_p(args...);
}
} // namespace auby

#define AUBY_CONCAT_(x, y) x##y
#define AUBY_CONCAT(x, y) AUBY_CONCAT_(x, y)

#define $hook(target, detour)                                                  \
    [[gnu::constructor]] void AUBY_CONCAT(_registerHook, __COUNTER__)() {      \
        auby::hook<&detour>(#target);                                          \
    }
