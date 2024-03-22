#pragma once

#include <functional>
#include <map>
#include <string>

namespace auby::internal {
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
} // namespace auby::internal
