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

template <const auto T>
void hook(std::string sig) {
    internal::hook(internal::resolve(sig), (void*)T);
}

template <const auto T, typename... Args>
static inline auto orig(Args... args) -> internal::fn_t<decltype(T)>::ret {
    auto fn = (decltype(T))internal::orig((void*)T);
    return fn(args...);
}
} // namespace auby

#define AUBY_CONCAT_(x, y) x##y
#define AUBY_CONCAT(x, y) AUBY_CONCAT_(x, y)

#define $hook(target, detour)                                                  \
    [[gnu::constructor]] void AUBY_CONCAT(_registerHook, __COUNTER__)() {      \
        auby::hook<&detour>(#target);                                          \
    }
