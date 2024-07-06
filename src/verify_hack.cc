#include <auby.hh>

namespace {
bool $(EditLevelLayer::init)(EditLevelLayer* self, GJGameLevel* level) {
    level->m_isVerified[0] = 1;
    return $orig(self, level);
}
} // namespace
