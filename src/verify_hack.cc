#include "settings.hh"
#include <auby.hh>

namespace {
bool $(EditLevelLayer::init)(EditLevelLayer* self, GJGameLevel* level) {
    spdlog::info("{}", settings::level::verifyHack());
    if (settings::level::verifyHack())
        level->m_isVerified[0] = 1;

    return $orig(self, level);
}
} // namespace
