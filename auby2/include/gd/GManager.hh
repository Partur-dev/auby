#pragma once

#include <auby.hh>

using namespace cocos2d;

class GManager : CCNode {
    std::string m_fileName;
    bool m_setup;
    bool m_saved;
    bool m_quickSave;
};
