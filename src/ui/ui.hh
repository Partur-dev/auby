#pragma once

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
        bool* value;
    };

    struct Tab {
        std::string name;
        std::vector<Element*> elements;

        Tab(std::string name);
        Tab* toggle(std::string name, bool* value);
    };

    CCLayer* m_layer = nullptr;
    CCScale9Sprite* m_bg = nullptr;
    CCSize m_size = {500, 275};
    uint padding = 10;
    std::vector<Tab*> m_tabs;

    void init();
    Tab* addTab(std::string name);
    void setTab(Tab* tab);
    void loadTabs();
    void createBg();
    void createSidebar();
};
