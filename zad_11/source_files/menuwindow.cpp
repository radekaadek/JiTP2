#include "menuwindow.h"

/**
 * @brief Callback function for buttons using actionDescriptor
 * 
*/
void actionDescriptor::cb_action(Graph_lib::Address, Graph_lib::Address pAD)
{
    actionDescriptor *pDsc = reinterpret_cast<actionDescriptor *>(pAD);
    pDsc->pParent->menuAction(pDsc);
};

/**
 * @brief Create MenuItem. To attach see MenuItem::attach
 * 
 */
MenuItem::MenuItem(Graph_lib::Point loc, int w, int h, const std::string &label) :
    Graph_lib::Button(loc, w, h, label, nullptr){}

/**
 * @brief Attach MenuItem to MenuWindow and MenuHeader
 *
 * @param pWnd Pointer to MenuWindow
 * @param pMenu Pointer to MenuHeader
 * @param color Color of MenuItem
 * @param act Action to be performed on click
 */
void MenuItem::attach(MenuWindow *pWnd, MenuHeader *pMenu, Graph_lib::Color color = Graph_lib::Color::invisible, actionDescriptor::Action act = actionDescriptor::NoAction)
{
    buttonAction.pParent = pWnd;
    buttonAction.pMenu = pMenu;
    buttonAction.menu_action = act;
    buttonAction.selected_color = color;

    pWnd->attach(*this);
    pw->callback(reinterpret_cast<Fl_Callback *>(actionDescriptor::cb_action), &buttonAction);
}

/**
 * @brief Create MenuHeader. To attach see MenuHeader::attach
 * 
 */
MenuHeader::MenuHeader(Graph_lib::Point xy, int w, int h, const std::string &label) :
    Graph_lib::Button(xy, w, h, label, actionDescriptor::cb_action)
    {}
/**
 * @brief Destroy MenuHeader and all MenuItems
 * 
*/
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

/**
 * @brief Attach MenuHeader to MenuWindow and create MenuItems
 *
 * @param pWnd Pointer to MenuWindow
 * @param colors Vector of colorSpecs
 * @param act Action to be performed on click
 */
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

/**
 * @brief Hide all MenuItems
 *
 */
void MenuHeader::hideMenu()
{
    for (auto btn : btns)
    {
        btn->hide();
    }
    expanded = false;
}

/**
 * @brief Show all MenuItems
 *
 */
void MenuHeader::showMenu()
{
    for (auto btn : btns)
    {
        btn->show();
    }
    expanded = true;
}

/**
 * @brief Toggle MenuItems visibility
 *
 */
void MenuHeader::toggleOpen()
{
    expanded ? hideMenu() : showMenu();
}

/**
 * @brief Create MenuWindow with MenuHeaders
 * 
 */
MenuWindow::MenuWindow(Graph_lib::Point loc, int w, int h, const std::string &label) :
    Graph_lib::Window(loc, w, h, label),
    rect(Graph_lib::Point(50, 100), 300, 200),
    close_btn(Graph_lib::Point(x_max() - 70, 0), 70, 20, "Close", cb_close),
    menu_fill(Graph_lib::Point(x_max() - 200, 50), 80, 20, "Fill"),
    menu_line(Graph_lib::Point(x_max() - 100, 50), 80, 20, "Frame")
    {
        attach(close_btn);
        attach(rect);
        menu_fill.attach(this, fill_colors, actionDescriptor::Menu_select_fill);
        menus.push_back(&menu_fill);
        menu_line.attach(this, fill_colors, actionDescriptor::Menu_select_line);
        menus.push_back(&menu_line);
    }

/**
 * @brief Colors and names for MenuItems
 * 
 */
const std::vector<colorSpec> MenuWindow::fill_colors = {
    {"Red", Graph_lib::Color::red},
    {"Blue", Graph_lib::Color::blue},
    {"Green", Graph_lib::Color::green},
    {"Yellow", Graph_lib::Color::yellow},
    {"Black", Graph_lib::Color::black},
    {"White", Graph_lib::Color::white},
    {"Cyan", Graph_lib::Color::cyan},
    {"Magenta", Graph_lib::Color::magenta},
};

/**
 * @brief Set rectangle outline color
 * 
 * @param color Color to be set
 */
void MenuWindow::setRectOutlineColor(Graph_lib::Color color)
{
    rect.set_color(color);
    redraw();
}

/**
 * @brief Set rectangle fill color
 * 
 * @param color Color to be set
*/
void MenuWindow::setRectFillColor(Graph_lib::Color color)
{
    rect.set_fill_color(color);
    redraw();
}

/**
 * @brief Close all menus except the one passed as argument.
 *
 * @param pMenu Pointer to the menu that should not be closed.
 */
void MenuWindow::closeMenus(MenuHeader *pMenu = nullptr)
{
    for (auto menu : menus)
    {
        if (menu != pMenu)
        {
            menu->hideMenu();
        }
    }
}

/**
 * @brief Handle menu actions.
 *
 * @param action Pointer to the action descriptor.
 */
void MenuWindow::menuAction(actionDescriptor *action)
{
    switch (action->menu_action)
    {
    case actionDescriptor::Menu_toggle:
        action->pParent->closeMenus(action->pMenu);
        action->pMenu->toggleOpen();
        break;
    case actionDescriptor::Menu_select_fill:
        setRectFillColor(action->selected_color);
        break;
    case actionDescriptor::Menu_select_line:
        setRectOutlineColor(action->selected_color);
        break;
    case actionDescriptor::NoAction:
        break;
    }
}

/**
 * @brief Callback function for close button.
 *
 * @param pw Pointer to the window.
 */
void MenuWindow::cb_close(Graph_lib::Address, Graph_lib::Address pw)
{
    Graph_lib::reference_to<MenuWindow>(pw).hide();
}
