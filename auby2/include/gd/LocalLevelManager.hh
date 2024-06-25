#pragma once

#include <auby.hh>

using namespace cocos2d;

class LocalLevelManager : GManager {
    static LocalLevelManager* sharedState();

    CCArray* m_localLevels;
    CCArray* m_localLists;
    std::map<int, std::string> m_mainLevels;
};
