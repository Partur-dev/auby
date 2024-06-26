# TODO: refactor this old shit

from keystone import *
from capstone import *

ks = Ks(KS_ARCH_ARM64, KS_MODE_LITTLE_ENDIAN)
cs = Cs(CS_ARCH_ARM64, CS_MODE_LITTLE_ENDIAN)


def assemble(asm: list[str]) -> str:
    result = ""

    for i in asm:
        encoding, _ = ks.asm(i)
        result += bytes(encoding).hex()

    return result


def disassemble(asm: str) -> list[str]:
    result = []

    for i in cs.disasm(
        bytes.fromhex(asm),
        0x0,
    ):
        result.append(format("%s\t%s\n" % (i.mnemonic, i.op_str)).strip())

    return result


def assemble_jmp(addr: int, t: int, n: int) -> str:
    # skip x11 cuz im #0 x11 hater
    jmp_template = [
        "mov x11, x30",  # bl moves addr of next instruction to x30 so we must backup it
        f"bl #{t - addr - 4}",
    ]

    return assemble(jmp_template)


with open("bin/_GeometryJump", "rb") as f:
    content = f.read()


def read_orig(addr: int, len: int) -> str:
    return content[addr : int(addr + len / 2)].hex()


def patch(index, replacement):
    global content
    replacement = bytes.fromhex(replacement)
    new_content = content[:index] + replacement + content[index + len(replacement) :]
    content = new_content


count = 0
get_hook = 0x62AD34  # stupid random ugly function

codegen = "#include <auby.hh>\n"
codegen += "namespace origs {\n"
conds = ""
origs = ""


def create_hook(ret: str, name: str, args: list[str], addr: int) -> str:
    # php moment
    global count
    global codegen
    global conds
    global origs
    count += 1

    args = ", ".join(args)
    n = name.replace("::", "_")

    hex = assemble_jmp(addr, get_hook, count)

    codegen += f"long _{n};\n"
    codegen += f"__attribute__((naked)) {ret} {n}({args}) {{\n"
    codegen += "\tasm volatile(\n"

    orig_instr = disassemble(read_orig(addr, len(hex)))

    for inst in orig_instr:
        codegen += f'\t\t"{inst}\\n"\n'

    codegen += '\t\t"br %[ptr]\\n"\n'
    codegen += f'\t\t: : [ptr]"r"(_{n})\n'
    codegen += "\t);\n"
    codegen += "}\n"

    conds += f'\tif (sig == "{name}") return auby::internal::base() + {addr};\n'
    origs += f'\torigs::_{n} = auby::internal::resolve("{name}") + 8;\n'
    origs += f'\tm->insert_or_assign((void *)auby::internal::resolve("{name}"), (void *)&origs::{n});\n'

    patch(addr, hex)


# patch(0x3E4CFC, assemble(["cmp w0, #0", "b.hi #0x94"]))
# patch(0x3E4EE0, assemble(["nop", "nop"]))

patch(
    get_hook,
    assemble(
        [
            "ldr x9, =0x3",
            "lsl x9, x9, #32",
            "ldr x10, =0x20000000",
            "orr x9, x9, x10",
            "ldr x9, [x9]",
            "br x9",
        ]
    ),
)


create_hook("bool", "MenuLayer::init", ["void* self"], 0x27604c)
create_hook("bool", "LoadingLayer::init", ["void* self", "bool fromReload"], 0x1dfa60)
create_hook(
    "bool", "GameManager::isIconUnlocked", ["void* self", "int a1", "int a2"], 0x3235D4
)
create_hook(
    "bool", "GameManager::isColorUnlocked", ["void* self", "int a1", "int a2"], 0x3239A4
)
# create_hook("bool", "PauseLayer::init", ["void* self"], 0x14ABC4)
# create_hook("bool", "PlayLayer::init", ["void* self, bool a1, bool b2"], 0x116610)
# create_hook(
#     "void",
#     "PlayLayer::destroyPlayer",
#     ["void* self", "void* player", "void* object"],
#     0x120084,
# )

codegen += "}\n\n"
codegen += "uintptr_t auby::internal::resolve(std::string sig) {\n"
codegen += conds
codegen += "\treturn -1;\n"
codegen += "}\n\n"

codegen += "void auby::internal::load_origs(std::map<void*, void*>* m) {\n"
codegen += origs
codegen += "}\n"

with open("src/generated.cc", "w") as f:
    f.write(codegen)

with open("bin/GeometryJump", "wb") as f:
    f.write(content)
