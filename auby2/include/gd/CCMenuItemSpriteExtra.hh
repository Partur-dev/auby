#pragma once

#include <auby.hh>

using namespace cocos2d;

class CCMenuItemSpriteExtra : public CCMenuItemSprite {
public:
    static CCMenuItemSpriteExtra* create(
        CCNode* s1,
        CCNode* s2,
        CCObject* target,
        SEL_MenuHandler selector
    );

    template <auto fn>
    static CCMenuItemSpriteExtra* create(CCNode* spr) {
        static auto node = CCNode::create();

        struct Callback {
            void callback(CCObject*) {
                fn();
            }
        };

        return CCMenuItemSpriteExtra::create(
            spr,
            spr,
            node,
            menu_selector(Callback::callback)
        );
    }
};
