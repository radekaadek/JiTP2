#include "menuwindow.h"

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

void MenuWindow::setColor(Graph_lib::Color color)
{
    rect.set_color(color);
    redraw();
}

void MenuHeader::cb_openclose(Graph_lib::Address , Graph_lib::Address pMenu)
{
    MenuHeader *pMH = reinterpret_cast<MenuHeader *>(pMenu);
    pMH->openClose();
}


void MenuHeader::openClose()
{
    expanded ? hideMenu() : showMenu();
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

void MenuHeader::attach(MenuWindow *pWnd, const std::vector<colorSpec> &colors)
{
    this->pWnd = pWnd;
    Graph_lib::Point xy = this->loc;
    for (auto color : colors)
    {
        xy.y += this->height;
        Graph_lib::Button *btn = new Graph_lib::Button(xy, this->width, this->height, color.label, nullptr);
        btns.push_back(btn);
        pWnd->attach(*btn);
    }
    hideMenu();
    pw->callback(reinterpret_cast<Fl_Callback *>(cb_openclose), this);
}

MenuWindow::MenuWindow(Graph_lib::Point loc, int w, int h, const std::string &label) :
        Graph_lib::Window(loc, w, h, label),
        rect(Graph_lib::Point(100, 100), 300, 300),
        close_btn(Graph_lib::Point(x_max() - 70, 0), 70, 20, "Close", cb_close),
        btn_fill(Graph_lib::Point(0, 0), 70, 20, "Fill")
{
    rect.set_color(Graph_lib::Color::red);
    attach(close_btn);
    attach(rect);
    attach(btn_fill);
    btn_fill.attach(this, fill_colors);
}
