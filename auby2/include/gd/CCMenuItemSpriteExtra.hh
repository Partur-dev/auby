#pragma once

#include <auby.hh>
#include <spdlog/spdlog.h>

using namespace cocos2d;

static void test() {
    spdlog::error("unhandled error");
}

class CCMenuItemSpriteExtra : public CCMenuItemSprite {
public:
    static CCMenuItemSpriteExtra* create(
        CCNode* s1,
        CCNode* s2,
        CCObject* target,
        SEL_MenuHandler selector
    );

    template <auto fn, class... Args>
    static CCMenuItemSpriteExtra* create(CCNode* sprite, Args... args) {
        struct Callback {
            void callback(CCNode* obj) {
                auto args = (std::tuple<Args...>*)this;
                std::apply(fn, *args);
            }
        };

        auto obj = new std::tuple(args...);

        return CCMenuItemSpriteExtra::create(
            sprite,
            sprite,
            (CCNode*)obj,
            menu_selector(Callback::callback)
        );
    }
};
