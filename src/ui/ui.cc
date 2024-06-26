#include "ui.hh"

UI& UI::get() {
    static UI ui;
    ui.init();
    return ui;
}

void UI::show(CCLayer* layer) {
    layer->addChild(m_layer, 9999);
}

UI::Tab::Tab(std::string name) {
    this->name = name;
}

UI::Tab* UI::Tab::toggle(std::string name, bool* value) {
    auto el = new UI::ToggleElement();
    el->name = name;
    el->value = value;
    this->elements.push_back(el);
    return this;
}

void UI::init() {
    m_layer = CCLayer::create();
    m_layer->setTouchEnabled(true);
    m_layer->setTouchPriority(10);

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    m_layer->setPosition({winSize.width / 2, winSize.height / 2});

    loadTabs();
    createBg();
    createSidebar();
}

UI::Tab* UI::addTab(std::string name) {
    auto tab = new UI::Tab(name);
    m_tabs.push_back(tab);
    return tab;
}

void UI::setTab(Tab* tab) {
    spdlog::info("Setting tab {}", tab->name);
}

void UI::loadTabs() {
    bool test;
    addTab("Bypass")->toggle("Test", &test);
    addTab("Global")->toggle("Test", &test);
    addTab("Level")->toggle("Test", &test);
    addTab("Player")->toggle("Test", &test);
}

void UI::createBg() {
    m_bg = CCScale9Sprite::create("GJ_square01.png", CCRectZero);
    m_bg->setContentSize(m_size);
    m_layer->addChild(m_bg);
}

void UI::createSidebar() {
    auto sidebar = CCScale9Sprite::create("square02_small.png", CCRectZero);
    sidebar->setOpacity(75);
    sidebar->setContentSize({120, m_size.height - padding * 2});
    sidebar->setPosition(ccp(padding, padding));
    sidebar->setAnchorPoint({0, 0});
    m_bg->addChild(sidebar);

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    sidebar->addChild(menu);

    for (auto* tab : m_tabs) {
        static int btnOffset = padding * 2;

        auto spr = ButtonSprite::create(
            tab->name.c_str(),
            120 - padding * 2 - 5,
            true,
            "goldFont.fnt",
            "GJ_button_01.png",
            .0f,
            .75f
        );

        auto btn = CCMenuItemSpriteExtra::create<[](auto setTab) {
            setTab();
        }>(spr, std::bind(&UI::setTab, this, tab));

        btn->setPosition(ccp(sidebar->getPosition().x + 60 - padding, btnOffset));
        menu->addChild(btn);

        btnOffset += btn->getContentSize().height + 5;
    }
}
