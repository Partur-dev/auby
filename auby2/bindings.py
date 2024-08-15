# https://github.com/CallocGD/PyBroma
from pybroma import Root, FunctionBindField, Class, PadField, MemberField
import os

os.chdir(os.path.dirname(os.path.abspath(__file__)))

INCLUDES_TEMPLATE = """struct TodoReturnPlaceholder {};

#include <unordered_set>

#include <cocos-ext.h>
#include <cocos2d.h>
using namespace cocos2d;
using namespace cocos2d::extension;
#include <fmod/placeholder.hh>
#include <gd/gd.hh>

#include <auby/internal.hh>
"""

FN_TEMPLATE = """{ret} {name}({args}) {const}{{
    static auto fn = reinterpret_cast<{ret} (*)({static_args})>(auby::internal::base() + {addr});
    return fn({call_args});
}}"""

FN_CTOR_TEMPLATE = ""  # TODO

CLASS_TEMPLATE = """class {name} {extends}{{
public:
    {name}();
    ~{name}();
{functions}

{fields}
}};"""
FN_PROTO_TEMPLATE = "    {static}{virtual}{ret} {name}({args}){const};"
FN_CTOR_PROTO_TEMPLATE = ""  # TODO
FIELD_TEMPLATE = "    {type} {name};"
PAD_TEMPLATE = "    char __pad{n}[{size}];"


def fix_type(t: str) -> str:
    return (
        t.replace("gd::", "std::")
        .replace("geode::SeedValueVRS", "std::array<int, 3>")
        .replace("geode::SeedValueVSR", "std::array<int, 3>")
        .replace("geode::SeedValueRVS", "std::array<int, 3>")
        .replace("geode::SeedValueRSV", "std::array<int, 3>")
        .replace("geode::SeedValueSVR", "std::array<int, 3>")
        .replace("geode::SeedValueSRV", "std::array<int, 3>")
        .replace("geode::SeedValueRS", "std::array<int, 2>")
        .replace("geode::SeedValueSR", "std::array<int, 2>")
    )


def gen_bind(c: Class, m: FunctionBindField) -> str:
    call_args = m.prototype.args
    static_args = [f"{fix_type(m.prototype.args[n].name)}" for n in m.prototype.args]

    if not m.prototype.is_static:
        call_args = ["this", *call_args]
        static_args = [
            f"{c.name} const*" if m.prototype.is_const else f"{c.name}*",
            *static_args,
        ]

    return (FN_TEMPLATE if m.prototype.ret.name else FN_CTOR_TEMPLATE).format(
        name=f"{c.name}::{m.prototype.name}",
        ret=fix_type(m.prototype.ret.name or "void"),
        const="const " if m.prototype.is_const else "",
        addr=hex(m.binds.ios),
        args=", ".join(
            f"{fix_type(m.prototype.args[n].name)} {n}" for n in m.prototype.args
        ),
        static_args=", ".join(static_args),
        call_args=", ".join(call_args),
    )


def gen_proto(m: FunctionBindField) -> str:
    return (
        FN_PROTO_TEMPLATE if m.prototype.ret.name else FN_CTOR_PROTO_TEMPLATE
    ).format(
        ret=fix_type(m.prototype.ret.name or "void"),
        name=m.prototype.name,
        args=", ".join(
            f"{fix_type(m.prototype.args[n].name)} {n}" for n in m.prototype.args
        ),
        static="static " if m.prototype.is_static else "",
        virtual="virtual " if m.prototype.is_virtual else "",
        const=" const" if m.prototype.is_const else "",
    )


def gen_field(mf: MemberField) -> str:
    return FIELD_TEMPLATE.format(type=fix_type(mf.type.name), name=mf.name)


pad_count = 0


def gen_pad(pf: PadField) -> str:
    global pad_count
    pad_count += 1
    return PAD_TEMPLATE.format(n=pad_count, size=pf.amount.ios)


binds = []
header_prolog = []
header = []

for c in Root("deps/bindings/bindings/2.206/Cocos2d.bro").classes:
    for f in c.fields:
        m = f.getAsFunctionBindField()
        # TODO: handle inline
        if m != None:
            if m.binds.ios > 0:
                binds.append(gen_bind(c, m))

for c in Root("deps/bindings/bindings/2.206/Extras.bro").classes:
    functions = []
    fields = []
    header_prolog.append(f"struct {c.name};")

    for f in c.fields:
        if mf := f.getAsMemberField():
            fields.append(gen_field(mf))

        if pf := f.getAsPadField():
            if pf.amount.ios > 0:
                fields.append(gen_pad(pf))

    header.append(
        CLASS_TEMPLATE.format(
            name=c.name,
            extends=(
                f': {", ".join(f'public {x}' for x in c.superclasses)} '
                if len(c.superclasses) > 0
                else ""
            ),
            functions="\n".join(functions),
            fields="\n".join(fields),
        )
    )

for c in Root("deps/bindings/bindings/2.206/GeometryDash.bro").classes:
    functions = []
    fields = []
    header_prolog.append(f"struct {c.name};")

    for f in c.fields:
        if m := f.getAsFunctionBindField():
            if m.binds.ios > 0:
                binds.append(gen_bind(c, m))
            functions.append(gen_proto(m))

        if mf := f.getAsMemberField():
            fields.append(gen_field(mf))

        if pf := f.getAsPadField():
            if pf.amount.ios > 0:
                fields.append(gen_pad(pf))

    header.append(
        CLASS_TEMPLATE.format(
            name=c.name,
            extends=(
                f': {", ".join(f'public {x}' for x in c.superclasses)} '
                if len(c.superclasses) > 0
                else ""
            ),
            functions="\n".join(functions),
            fields="\n".join(fields),
        )
    )

with open("src/generated_binds.cc", "w") as f:
    f.write(f"{INCLUDES_TEMPLATE}\n\n" + "\n\n".join(binds))

with open("include/gd/generated.hh", "w") as f:
    f.write(
        "#include <cocos2d.h>\n\n"
        + "\n".join(header_prolog)
        + "\n\n"
        + "\n\n".join(header)
    )
