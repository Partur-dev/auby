#pragma once

#include "auby/settings.hh"
#include <auby.hh>

class UI {
public:
    static UI& get();

    void show(CCLayer* layer);

private:
    class Element {
        virtual void _() {}
    };

    struct ToggleElement : Element {
        std::string name;
        auby::SettingsValue<bool>& value;

        ToggleElement(std::string name, auby::SettingsValue<bool>& value) :
            name(name),
            value(value) {}
    };

    struct Tab {
        std::string name;
        std::vector<Element*> elements;

        Tab(std::string name);
        Tab* toggle(std::string name, auby::SettingsValue<bool>& value);
    };

    CCLayer* m_layer = nullptr;
    CCScale9Sprite* m_bg = nullptr;
    CCMenu* m_content = nullptr;
    CCSize m_size = {500, 275};
    uint padding = 10;
    std::vector<Tab*> m_tabs;

    UI();
    Tab* addTab(std::string name);
    void setTab(Tab* tab);
    void loadTabs();
    void createBg();
    void createSidebar();
};
