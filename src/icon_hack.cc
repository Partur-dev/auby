#include <auby.hh>

namespace {
bool GameManager_isIconUnlocked(void* self, int a1, int a2) {
    return true;
}

bool GameManager_isColorUnlocked(void* self, int a1, int a2) {
    return true;
}

$hook(GameManager::isIconUnlocked, GameManager_isIconUnlocked);
$hook(GameManager::isColorUnlocked, GameManager_isIconUnlocked);
} // namespace
