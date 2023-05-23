#pragma once
#include "GUI.h"

struct colorSpec
{
  std::string label;
  Graph_lib::Color color;
};

class MenuWindow;
class MenuHeader;
class MenuItem;

struct actionDescriptor
{
  enum Action
  {
    NoAction,
    Menu_toggle,
    Menu_select_fill,
    Menu_select_line
  };
  MenuWindow *pParent = nullptr;
  MenuHeader *pMenu = nullptr;
  Action menu_action = NoAction;
  Graph_lib::Color selected_color = Graph_lib::Color::invisible;
  static void cb_item(Graph_lib::Address, Graph_lib::Address pAD);
};

class MenuItem : public Graph_lib::Button
{

public:
  MenuItem(Graph_lib::Point loc, int w, int h, const std::string &label) : Graph_lib::Button(loc, w, h, label, nullptr)
  {
  }
  void attach(MenuWindow *pWnd, MenuHeader *pMenu, Graph_lib::Color color, actionDescriptor::Action act);

private:
  actionDescriptor buttonAction;
};

class MenuHeader : public Graph_lib::Button
{
  std::vector<Graph_lib::Button *> btns;
  MenuWindow *pWnd = nullptr;
  bool expanded = false;
  actionDescriptor mAction;

public:
  MenuHeader(Graph_lib::Point xy, int w, int h, const std::string &label) : Graph_lib::Button(xy, w, h, label, actionDescriptor::cb_item)
  {
  }
  ~MenuHeader();
  void toggleOpen();
  void hideMenu();
  void showMenu();
  void attach(MenuWindow *pWnd, const std::vector<colorSpec> &btns, actionDescriptor::Action action);
};

class MenuWindow : public Graph_lib::Window
{
  Graph_lib::Rectangle rect;
  Graph_lib::Button close_btn;
  MenuHeader menu_fill;
  MenuHeader menu_line;

  static void cb_close(Graph_lib::Address, Graph_lib::Address pw)
  {
    Graph_lib::reference_to<MenuWindow>(pw).hide();
  }

public:
  MenuWindow(Graph_lib::Point loc, int w, int h, const std::string &label);
  const static std::vector<colorSpec> fill_colors;
  void setFillColor(Graph_lib::Color color);
  void setOutlineColor(Graph_lib::Color color);
  void menuAction(actionDescriptor *action);
  void closeMenus();
};
