#include <auby.hh>

CCMenuItemSpriteExtra* CCMenuItemSpriteExtra::create(
    CCNode* s1,
    CCNode* s2,
    CCObject* target,
    SEL_MenuHandler selector
) {
    return auby::internal::call<CCMenuItemSpriteExtra*, 0x223cdc>(
        s1,
        s2,
        target,
        selector
    );
}
