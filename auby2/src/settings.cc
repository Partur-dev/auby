#include <auby/settings.hh>

namespace auby {
Settings* Settings::shared() {
    static auto inst = new Settings();
    return inst;
}
} // namespace auby
