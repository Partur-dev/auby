#include <auby.hh>

CCSpriteFrameCache* CCSpriteFrameCache::sharedSpriteFrameCache() {
    return auby::internal::call<CCSpriteFrameCache*, 0x3c4cf0>();
}

void CCSpriteFrameCache::addSpriteFramesWithFile(const char* file) {
    return auby::internal::call<void, 0x3c5734>(this, file);
}

CCTextureCache* CCTextureCache::sharedTextureCache() {
    return auby::internal::call<CCTextureCache*, 0x114948>();
}

CCTexture2D* CCTextureCache::addImage(const char* file, bool val) {
    return auby::internal::call<CCTexture2D*, 0x115540>(this, file, val);
}

CCObject* CCArray::objectAtIndex(unsigned int i) {
    return auby::internal::call<CCObject*, 0x259c24>(this, i);
}

CCLabelBMFont* CCLabelBMFont::create(const char* text, const char* font) {
    return auby::internal::call<CCLabelBMFont*, 0x30b814>(text, font);
}

void CCNode::addChild(CCNode* child) {
    return auby::internal::call<void, 0x24bed8>(this, child);
}

CCDirector* CCDirector::sharedDirector() {
    return auby::internal::call<CCDirector*, 0x180c2c>();
}

CCSize CCDirector::getWinSize() {
    return auby::internal::call<CCSize, 0x1818b4>(this);
}

CCSprite* CCSprite::createWithSpriteFrameName(const char* frame) {
    return auby::internal::call<CCSprite*, 0x247d9c>(frame);
}

CCMenu* CCMenu::create() {
    return auby::internal::call<CCMenu*, 0x1e1940>();
}
