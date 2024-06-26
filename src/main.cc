#include "ui/ui.hh"
#include <auby.hh>

namespace {
bool $(LoadingLayer::init)(LoadingLayer* self, bool fromReload) {
    $orig(self, fromReload);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto label = CCLabelBMFont::create("Auby v2.0.0", "goldFont.fnt");
    label->setPosition(winSize.width / 2, 40);
    label->setScale(.50);
    self->addChild(label);

    return true;
}

bool $(MenuLayer::init)(MenuLayer* self) {
    $orig(self);

    auto btn = CCMenuItemSpriteExtra::create<[](auto self) {
        UI::get().show(self);
    }>(CCSprite::createWithSpriteFrameName("pathIcon_10_001.png"), self);

    auto menu = CCMenu::create();
    self->addChild(menu);
    menu->setPosition({125, 100});
    menu->addChild(btn);

    return true;
}
} // namespace
