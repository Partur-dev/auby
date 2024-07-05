#include <auby.hh>

ButtonSprite* ButtonSprite::create(
    const char* caption,
    int width,
    bool absolute,
    const char* font,
    const char* texture,
    float height,
    float scale
) {
    return ButtonSprite::create(caption, width, 0, scale, absolute, font, texture, height);
}
