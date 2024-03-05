#include <auby.hh>

CCMenuItemSpriteExtra* CCMenuItemSpriteExtra::create(
    CCNode* s1,
    CCNode* s2,
    CCObject* target,
    SEL_MenuHandler selector
) {
    static auto addr = auby::internal::base() + 0x21c4fc;
    using fn_t = CCMenuItemSpriteExtra* (*)(CCNode* s1,
                                            CCNode* s2,
                                            CCObject* target,
                                            SEL_MenuHandler selector);
    static auto fn = (fn_t)addr;
    return fn(s1, s2, target, selector);
}
