#pragma once

#include <auby.hh>

template <auto fn, class... Args>
CCMenuItemSpriteExtra* ccMenuItemSpriteExtraWithCallback(CCNode* sprite, Args... args) {
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
