#pragma once

#include <cocos2d.h>
#include <functional>
#include <gd/gd.hh>
#include <map>
#include <string>
#include <type_traits>

using namespace cocos2d;

namespace auby {
namespace internal {
void init();

uintptr_t base();
uintptr_t resolve(std::string sig);
void hook(uintptr_t target, void* detour);
void* orig(void* detour);
void load_origs(std::map<void*, void*>* m);

template <typename T>
struct fn_t {};

template <typename Ret, typename... Args>
struct fn_t<Ret (*)(Args...)> {
    using ret = Ret;
};
} // namespace internal

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

void auby_main();

#define init_auby()                                                            \
    [[gnu::constructor]] void __entry() {                                      \
        auby::internal::init();                                                \
        auby_main();                                                           \
    }
