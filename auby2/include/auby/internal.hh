#pragma once

#pragma clang optimize off
#include <functional>
#include <map>
#include <string>
#pragma clang optimize on

namespace auby::internal {
void init();

uintptr_t base();
uintptr_t resolve(std::string sig);
void hook(uintptr_t target, void* detour);
void* orig(void* detour);
void load_origs(std::map<void*, void*>* m);

template <typename T, unsigned int Addr, typename... Args>
static inline auto call(Args... args) {
    static auto fn = reinterpret_cast<T (*)(Args...)>(base() + Addr);
    return fn(args...);
}
} // namespace auby::internal
