#pragma once

#include <auby.hh>

struct ButtonSprite : cocos2d::CCSprite {
    static ButtonSprite* create(
        const char* a,
        int b,
        int c,
        float d,
        bool e,
        const char* f,
        const char* g,
        float h
    );

    inline static ButtonSprite* create(
        const char* caption,
        int width,
        bool absolute,
        const char* font,
        const char* texture,
        float height,
        float scale
    ) {
        return create(
            caption,
            width,
            0,
            scale,
            absolute,
            font,
            texture,
            height
        );
    }
};
