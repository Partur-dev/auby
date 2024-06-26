#include "ui/ui.hh"
#include <auby.hh>

namespace {
bool $(LoadingLayer::init)(LoadingLayer* self, bool fromReload) {
    $orig(self, fromReload);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto label = CCLabelBMFont::create("Auby v2.0.0", "goldFont.fnt");
    label->setPosition(winSize.width / 2, winSize.height / 2 - 25);
    label->setScale(.75);
    self->addChild(label);

    return true;
}

bool $(MenuLayer::init)(MenuLayer* self) {
    $orig(self);
    UI::get().show(self);
    return true;
}
} // namespace
