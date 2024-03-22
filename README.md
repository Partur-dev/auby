# Auby

Open Source & JB-free mods for Geometry Dash

## How hooks work

```asm
gdFunc:
  mov x11, x30 ; backup original link register
  bl trampoline

trampoline:
  ldr x9, =0x2
  lsl x9, x9, #32
  ldr x10, =0x80000000
  orr x9, x9, x10
  ldr x9, [x9]
  br x9 ; jump to 0x280000000
```

```cc
void *findTheHook() {
  void *target;
  asm("mov %[t], x16" : [t] "=r"(target));
  // now target contains [base + offset]
}

__attribute__((naked)) void findTheHookProxy() {
  asm volatile("mov x12, x0\n" // backup args
               "mov x13, x1\n"
               "mov x14, x2\n"
               "mov x15, x3\n"
               "mov x17, x4\n"
               "sub x30, x30, 8\n"
               "mov x16, x30\n"
               "bl %[findTheHook]\n" // find the hook
               "mov x30, x11\n" // restore x30 (fix ret)
               "mov x9, x0\n" // restore args
               "mov x0, x12\n"
               "mov x1, x13\n"
               "mov x2, x14\n"
               "mov x3, x15\n"
               "mov x4, x17\n"
               "br x9\n" ::[findTheHook] "i"(findTheHook));
}

void init() {
  *((void**)0x280000000) = (void*)&findTheHookProxy;
}
```

> [!IMPORTANT]  
> Clang optimization breaks auby, so `#pragma clang optimize off` is required

## Discord

https://discord.gg/kAUjNCEYcC
