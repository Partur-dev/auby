#include <auby.hh>

CCLabelBMFont* CCLabelBMFont::create(const char* text, const char* font) {
    static auto addr = auby::internal::base() + 0x301b04;
    using fn_t = CCLabelBMFont* (*)(const char* text, const char* font);
    static auto fn = (fn_t)addr;
    return fn(text, font);
}

void CCNode::addChild(CCNode* child) {
    static auto addr = auby::internal::base() + 0x24444c;
    using fn_t = void (*)(CCNode* self, CCNode* child);
    static auto fn = (fn_t)addr;
    return fn(this, child);
}

CCDirector* CCDirector::sharedDirector() {
    static auto addr = auby::internal::base() + 0x17bf5c;
    using fn_t = CCDirector* (*)();
    static auto fn = (fn_t)addr;
    return fn();
}

CCSize CCDirector::getWinSize() {
    static auto addr = auby::internal::base() + 0x17cbe4;
    using fn_t = CCSize (*)(CCDirector* self);
    static auto fn = (fn_t)addr;
    return fn(this);
}
