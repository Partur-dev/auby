#pragma once

#include <auby.hh>

using namespace cocos2d;

class CCMenuItemSpriteExtra : public CCMenuItemSprite {
private:
    template <auto fn>
    struct Callback {
        void callback() {
            fn();
        }
    };

public:
    static CCMenuItemSpriteExtra* create(
        CCNode* s1,
        CCNode* s2,
        CCObject* target,
        SEL_MenuHandler selector
    );

    template <auto fn>
    static CCMenuItemSpriteExtra* create(CCNode* spr, CCObject* target) {
        return CCMenuItemSpriteExtra::create(
            spr,
            spr,
            target,
            (SEL_MenuHandler)&Callback<fn>::callback
        );
    }
};
