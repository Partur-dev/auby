#include "ui.hh"

UI& UI::get() {
    static auto ui = UI();
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

UI::UI() {
    m_layer = CCLayer::create();
    m_layer->setTouchEnabled(true);
    m_layer->setTouchPriority(10);

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    m_layer->setPosition({winSize.width / 2, winSize.height / 2});

    loadTabs();
    spdlog::info("Loaded {} tabs", m_tabs.size());

    createBg();
    createSidebar();

    m_content = CCMenu::create();
    m_bg->addChild(m_content);

    setTab(m_tabs.at(0));
}

UI::Tab* UI::addTab(std::string name) {
    auto tab = new UI::Tab(name);
    m_tabs.push_back(tab);
    return tab;
}

void UI::setTab(Tab* tab) {
    spdlog::info("Setting tab {}", tab->name);

    // m_content->removeAllChildren();

    for (Element* el : tab->elements) {
        if (auto toggle = dynamic_cast<ToggleElement*>(el)) {
            auto btn = ccMenuItemSpriteExtraWithCallback<[](auto toggle) {
                *toggle->value = !*toggle->value;
            }>(CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png"), toggle);

            m_content->addChild(btn);
        }
    }
}

void UI::loadTabs() {
    bool test;
    addTab("Bypass");
    addTab("Global")->toggle("Test", &test);
    addTab("Level");
    addTab("Player");
}

void UI::createBg() {
    auto backdrop = CCScale9Sprite::create("square02_small.png", CCRectZero);
    backdrop->setOpacity(75);
    backdrop->setContentSize(CCDirector::get()->getWinSize());
    m_layer->addChild(backdrop);

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

    auto label = CCLabelBMFont::create("Auby", "bigFont.fnt");
    label->setScale(0.5);
    label->setPosition(
        ccp(sidebar->getContentSize().width / 2, sidebar->getContentSize().height - padding)
    );
    sidebar->addChild(label);

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    sidebar->addChild(menu);

    int btnOffset = padding * 2;
    for (auto* tab : m_tabs) {
        auto spr = ButtonSprite::create(
            tab->name.c_str(),
            120 - padding * 2 - 5,
            true,
            "goldFont.fnt",
            "GJ_button_01.png",
            .0f,
            .75f
        );

        auto btn = ccMenuItemSpriteExtraWithCallback<[](auto setTab, auto self, auto tab) {
            (self->*setTab)(tab);
        }>(spr, &UI::setTab, this, tab);

        btn->setPosition(ccp(sidebar->getPosition().x + 60 - padding, btnOffset));
        menu->addChild(btn);

        btnOffset += btn->getContentSize().height + 5;
    }
}
