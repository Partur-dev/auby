#include <auby.hh>

CCMenuItemSpriteExtra* CCMenuItemSpriteExtra::create(
    CCNode* s1,
    CCNode* s2,
    CCObject* target,
    SEL_MenuHandler selector
) {
    return auby::internal::call<CCMenuItemSpriteExtra*, 0x223CDC>(
        s1,
        s2,
        target,
        selector
    );
}

ButtonSprite* ButtonSprite::create(
    const char* a,
    int b,
    int c,
    float d,
    bool e,
    const char* f,
    const char* g,
    float h
) {
    return auby::internal::call<ButtonSprite*, 0x65d30>(a, b, c, d, e, f, g, h);
}
