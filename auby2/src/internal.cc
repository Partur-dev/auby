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

#define GET_REG(r)                                                                                 \
    void* r;                                                                                       \
    asm("mov %[t], " #r : [t] "=r"(r));
#define SET_REG(r) asm("mov " #r ", %[t]" ::[t] "r"(r));

void* findTheHook() {
    GET_REG(x16);
    // bekap
    GET_REG(x11);
    GET_REG(x12);
    GET_REG(x1);
    GET_REG(x2);
    GET_REG(x3);
    GET_REG(x4);

    void* ret = (void*)&stub;

    if (hooks()->contains(x16)) {
        ret = (void*)hooks()->at(x16);
        SET_REG(x11);
        SET_REG(x12);
        SET_REG(x1);
        SET_REG(x2);
        SET_REG(x3);
        SET_REG(x4);
    }

    return ret;
}

__attribute__((naked)) void findTheHookProxy() {
    asm volatile("mov x12, x0\n"
                 "sub x30, x30, 8\n"
                 "mov x16, x30\n"
                 "bl %[findTheHook]\n"
                 "mov x30, x11\n"
                 "mov x9, x0\n"
                 "mov x0, x12\n"
                 "br x9\n" ::[findTheHook] "i"(findTheHook));
}

namespace auby::internal {

void init() {
    auto expected = (void*)0x320000000;
    // this shit allocates memory at 0x320000000
    auto allocated = mmap(
        expected,
        sizeof(void*) * 100,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );

    if (allocated != expected) {
        spdlog::error(
            "FATAL ERROR: Allocated at 0x{0:x}, expected at 0x{0:x}",
            allocated,
            expected
        );
        exit(1);
    }

    *(void**)(expected) = (void*)&findTheHookProxy;

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
