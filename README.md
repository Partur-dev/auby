# Auby <img src="https://cdn.discordapp.com/icons/1213607627318239355/d7fee577b10e6cde20074a2e0dbcd2c6.webp?size=240" height="42" width="168">

Open Source & JB-free mods for Geometry Dash

---

<p align="center">
  <img src="https://count.akame.moe/@Partur-dev?theme=moebooru">
</p>

---

## 📱 Install

1. Compile auby or get **very very very outdated** dylib from releases
2. Get 2.206 decrypted IPA
3. Inject auby and sideload IPA (the easiest way is to use [sideloadly](https://sideloadly.io/))

## 📋 TODO

- Rewrite in Rust
- Rewrite patcher in normal language
- Load mod with dlopen
- Geode support

## ❓ FAQ

<details>
<summary>How Hooks Work</summary>

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

</details>

## 📄 License

Auby is free software licensed under the [GPL-3.0](LICENSE)

###### Partur • [Discord](https://discord.gg/kAUjNCEYcC) • [Telegram](https://t.me/gmd_ytro)
