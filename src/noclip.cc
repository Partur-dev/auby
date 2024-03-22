#include <auby.hh>

namespace {
static void* g_acObject = nullptr;
static bool g_noclip = false;

void PlayLayer_destroyPlayer(void* self, void* player, void* object) {
    if (!g_acObject)
        g_acObject = object;

    if (object == g_acObject || !g_noclip)
        auby::orig<&PlayLayer_destroyPlayer>(self, player, object);
}

bool PlayLayer_init(void* self, bool a1, bool a2) {
    auby::orig<&PlayLayer_init>(self, a1, a2);
    g_acObject = nullptr;
    return true;
}

void toggleNoclip() {
    g_noclip = !g_noclip;
}

bool PauseLayer_init(CCNode* self) {
    auby::orig<&PauseLayer_init>(self);

    auto spr = CCSprite::createWithSpriteFrameName("GJ_plus2Btn_001.png");
    auto item = CCMenuItemSpriteExtra::create<&toggleNoclip>(spr, self);

    auto menu = CCMenu::create();
    menu->setPosition({35, 35});

    menu->addChild(item);
    self->addChild(menu);

    return true;
}

$hook(PlayLayer::init, PlayLayer_init);
$hook(PlayLayer::destroyPlayer, PlayLayer_destroyPlayer);
$hook(PauseLayer::init, PauseLayer_init);
} // namespace
