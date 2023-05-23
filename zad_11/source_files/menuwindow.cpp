#include "menuwindow.h"

void actionDescriptor::cb_action(Graph_lib::Address, Graph_lib::Address pAD)
{
    actionDescriptor *pDsc = reinterpret_cast<actionDescriptor *>(pAD);
    pDsc->pParent->menuAction(pDsc);
};

MenuItem::MenuItem(Graph_lib::Point loc, int w, int h, const std::string &label) :
    Graph_lib::Button(loc, w, h, label, nullptr)
    {}

void MenuItem::attach(MenuWindow *pWnd, MenuHeader *pMenu, Graph_lib::Color color, actionDescriptor::Action act)
{
    buttonAction.pParent = pWnd;
    buttonAction.pMenu = pMenu;
    buttonAction.menu_action = act;
    buttonAction.selected_color = color;

    pWnd->attach(*this);
    pw->callback(reinterpret_cast<Fl_Callback *>(actionDescriptor::cb_action), &buttonAction);
}

MenuHeader::MenuHeader(Graph_lib::Point xy, int w, int h, const std::string &label) :
    Graph_lib::Button(xy, w, h, label, actionDescriptor::cb_action)
    {}

MenuHeader::~MenuHeader()
{
    if (pWnd)
    {
        pWnd->detach(*this);
    }
    for (auto btn : btns)
    {
        pWnd->detach(*btn);
        delete btn;
    }
}

void MenuHeader::attach(MenuWindow *pWnd, const std::vector<colorSpec> &colors, actionDescriptor::Action act)
{
    this->pWnd = pWnd;
    pWnd->attach(*this);
    Graph_lib::Point xy = this->loc;
    int h = this->height;
    for (auto col : colors)
    {
        xy.y += h;
        MenuItem *pItem = new MenuItem(xy, 80, 20, col.label);
        btns.push_back(pItem);
        pItem->attach(pWnd, this, col.color, act);
    }
    hideMenu();
    mAction.pParent = pWnd;
    mAction.pMenu = this;
    mAction.menu_action = actionDescriptor::Menu_toggle;
    pw->callback(reinterpret_cast<Fl_Callback *>(actionDescriptor::cb_action), &mAction);
}

void MenuHeader::hideMenu()
{
    for (auto btn : btns)
    {
        btn->hide();
    }
    expanded = false;
}

void MenuHeader::showMenu()
{
    for (auto btn : btns)
    {
        btn->show();
    }
    expanded = true;
}

void MenuHeader::toggleOpen()
{
    expanded ? hideMenu() : showMenu();
}

MenuWindow::MenuWindow(Graph_lib::Point loc, int w, int h, const std::string &label) :
    Graph_lib::Window(loc, w, h, label),
    rect(Graph_lib::Point(100, 100), 300, 200),
    close_btn(Graph_lib::Point(x_max() - 70, 0), 70, 20, "Close", cb_close),
    menu_fill(Graph_lib::Point(0, 0), 70, 20, "Fill"),
    menu_line(Graph_lib::Point(x_max() - 130, 50), 80, 20, "Frame")
    {
        attach(close_btn);
        attach(rect);
        menu_fill.attach(this, fill_colors, actionDescriptor::Menu_select_fill);
        menu_line.attach(this, fill_colors, actionDescriptor::Menu_select_line);
    }

const std::vector<colorSpec> MenuWindow::fill_colors = {
    {"red", Graph_lib::Color::red},
    {"blue", Graph_lib::Color::blue},
    {"green", Graph_lib::Color::green},
    {"yellow", Graph_lib::Color::yellow},
    {"black", Graph_lib::Color::black},
    {"white", Graph_lib::Color::white},
    {"cyan", Graph_lib::Color::cyan},
    {"magenta", Graph_lib::Color::magenta},
};

void MenuWindow::menuAction(actionDescriptor *action)
{
    switch (action->menu_action)
    {
    case actionDescriptor::Menu_toggle:
        action->pParent->closeMenus();
        action->pMenu->toggleOpen();
        break;
    case actionDescriptor::Menu_select_fill:
        setFillColor(action->selected_color);
        break;
    case actionDescriptor::Menu_select_line:
        setOutlineColor(action->selected_color);
        break;
    case actionDescriptor::NoAction:
        break;
    }
}

void MenuWindow::setOutlineColor(Graph_lib::Color color)
{
    rect.set_color(color);
    redraw();
}

void MenuWindow::setFillColor(Graph_lib::Color color)
{
    rect.set_fill_color(color);
    redraw();
}

void MenuWindow::closeMenus()
{
    menu_fill.hideMenu();
    menu_line.hideMenu();
}

void MenuWindow::cb_close(Graph_lib::Address, Graph_lib::Address pw)
{
    Graph_lib::reference_to<MenuWindow>(pw).hide();
}
