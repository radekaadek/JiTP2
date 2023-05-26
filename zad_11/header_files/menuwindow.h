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
  { NoAction, Menu_toggle, Menu_select_fill, Menu_select_line };
  MenuWindow *pParent = nullptr;
  MenuHeader *pMenu = nullptr;
  Action menu_action = NoAction;
  Graph_lib::Color selected_color = Graph_lib::Color::invisible;
  static void cb_action(Graph_lib::Address, Graph_lib::Address pAD);
};

class MenuItem : public Graph_lib::Button
{
public:
  MenuItem(Graph_lib::Point loc, int w, int h, const std::string &label);
  void attach(MenuWindow *pWnd, MenuHeader *pMenu, Graph_lib::Color color, actionDescriptor::Action act);

private:
  actionDescriptor buttonAction;
};

class MenuHeader : public Graph_lib::Button
{
public:
  MenuHeader(Graph_lib::Point xy, int w, int h, const std::string &label);
  ~MenuHeader();
  void attach(MenuWindow *pWnd, const std::vector<colorSpec> &btns, actionDescriptor::Action action);
  void hideMenu();
  void showMenu();
  void toggleOpen();
  bool isExpanded() const;
private:
  bool expanded = false;
  std::vector<Graph_lib::Button *> btns;
  MenuWindow *pWnd = nullptr;
  actionDescriptor mAction;
};

class MenuWindow : public Graph_lib::Window
{
public:
  MenuWindow(Graph_lib::Point loc, int w, int h, const std::string &label);
  const static std::vector<colorSpec> fill_colors;
  void setRectFillColor(Graph_lib::Color color);
  void setRectOutlineColor(Graph_lib::Color color);
  void closeMenus(MenuHeader *pMenu);
  void menuAction(actionDescriptor *action);

private:
  std::vector<MenuHeader *> menus;
  Graph_lib::Rectangle rect;
  Graph_lib::Button close_btn;
  MenuHeader menu_fill;
  MenuHeader menu_line;
  static void cb_close(Graph_lib::Address, Graph_lib::Address pw);
};
