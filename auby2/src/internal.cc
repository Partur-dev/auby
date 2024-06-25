#include <auby.hh>
#include <mach-o/dyld.h>
#include <map>
#include <sstream>
#include <sys/mman.h>

auto hooks() {
    static auto* shared = new std::map<void*, void*>();
    return shared;
}

auto origs() {
    static auto* shared = new std::map<void*, void*>();
    return shared;
}

bool stub() {
    void* target;
    asm("mov %[t], x16" : [t] "=r"(target));

    auto s = std::stringstream();
    s << "Cannot find hook " << std::hex << target << std::endl;

    auby::alert(s.str());
    return true;
}

void* findTheHook() {
    void* target;
    asm("mov %[t], x16" : [t] "=r"(target));

    if (hooks()->contains(target)) {
        return hooks()->at(target);
    }

    return (void*)&stub;
}

__attribute__((naked)) void findTheHookProxy() {
    asm volatile("mov x12, x0\n"
                 "mov x13, x1\n"
                 "mov x14, x2\n"
                 "mov x15, x3\n"
                 "mov x17, x4\n"
                 "sub x30, x30, 8\n"
                 "mov x16, x30\n"
                 "bl %[findTheHook]\n"
                 "mov x30, x11\n"
                 "mov x9, x0\n"
                 "mov x0, x12\n"
                 "mov x1, x13\n"
                 "mov x2, x14\n"
                 "mov x3, x15\n"
                 "mov x4, x17\n"
                 "br x9\n" ::[findTheHook] "i"(findTheHook));
}

namespace auby::internal {

void init() {
    // this shit allocates memory at 280000000
    mmap(
        (void*)0x190000000,
        sizeof(void*) * 100,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );

    *(void**)(0x280000000) = (void*)&findTheHookProxy;

    auby::internal::load_origs(hooks());
}

uintptr_t base() {
    return _dyld_get_image_vmaddr_slide(0) + 0x100000000;
}

void hook(uintptr_t target, void* detour) {
    static bool inited = false;
    if (!inited) {
        internal::init();
        inited = true;
    }

    origs()->insert_or_assign(detour, hooks()->at((void*)target));
    hooks()->insert_or_assign((void*)target, detour);
}

void* orig(void* detour) {
    return origs()->at(detour);
}

} // namespace auby::internal
