#pragma once
#include "Graph.h"
#include "Point.h"
#include "Window.h"
#include "GUI.h"
#include "Point.h"

#define Address void*


class Spinner  {
    Graph_lib::Button btn_up;
    Graph_lib::Button btn_down;
    Graph_lib::In_box radiusBox;
    int curVal;
    static void cb_up(Address, Address);
    static void cb_down(Address, Address);
    void up();
    void down();
public: 
    Spinner(Point loc, int startVal) 
      : btn_up(loc, 20, 20, "/\\", Spinner::cb_up, this),
        btn_down(Point(loc.x + 60, loc.y), 20, 20, "\\/",  
            Spinner::cb_down, this), 
        radiusBox(Point(loc.x + 25, loc.y), 30, 20, ""), 
        curVal(startVal) 
        {} 
    void attach(Graph_lib::Window& wnd);
}; 

