#include <auby.hh>

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

bool MenuLayer_init(CCNode* self) {
    auby::orig<&MenuLayer_init>(self);
    auto winSize = CCDirector::get()->getWinSize();

    auto label
        = CCLabelBMFont::create("Auby v2 Experimental Build", "goldFont.fnt");
    label->setPosition({winSize.width / 2, winSize.height - 75});
    self->addChild(label, 100);

    return true;
}

bool GameManager_isIconUnlocked(void* self, int a1, int a2) {
    return true;
}

bool GameManager_isColorUnlocked(void* self, int a1, int a2) {
    return true;
}

void auby_main() {
    auby::hook<&MenuLayer_init>("MenuLayer::init");
    auby::hook<&GameManager_isIconUnlocked>("GameManager::isIconUnlocked");
    auby::hook<&GameManager_isColorUnlocked>("GameManager::isColorUnlocked");
    auby::hook<&PlayLayer_init>("PlayLayer::init");
    auby::hook<&PlayLayer_destroyPlayer>("PlayLayer::destroyPlayer");
    auby::hook<&PauseLayer_init>("PauseLayer::init");
}

init_auby();
