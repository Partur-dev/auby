#include <auby.hh>

namespace {
bool MenuLayer_init(CCNode* self) {
    auby::orig<&MenuLayer_init>(self);
    auto winSize = CCDirector::get()->getWinSize();

    auto label
        = CCLabelBMFont::create("Auby v2 Experimental Build", "goldFont.fnt");
    label->setPosition({winSize.width / 2, winSize.height - 75});
    self->addChild(label, 100);

    return true;
}

$hook(MenuLayer::init, MenuLayer_init);
} // namespace
