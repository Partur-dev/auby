#include <auby.hh>

// static void *g_acObject = 0;
// void PlayLayer_destroyPlayer(void *self, void *player, void *object) {
//   if (!g_acObject)
//     g_acObject = object;

//   if (object == g_acObject)
//     auby::orig<&PlayLayer_destroyPlayer>(self, player, object);
// }

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
}

init_auby();
