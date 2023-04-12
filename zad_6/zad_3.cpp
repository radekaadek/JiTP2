#include "Simple_window.h"

void cb_close(Address, Address pw)
{
    reference_to<Graph_lib::Window>(pw).hide();
}

class Spinner;

class SpButton : public Graph_lib::Button
{
    Spinner *pSpinner = nullptr;

public:
    SpButton(Point loc, int w, int h, const string &label,
             Graph_lib::Callback cb, Spinner *pSpin)
        : Graph_lib::Button(loc, w, h, label, cb), pSpinner(pSpin) {}
    void attach(Graph_lib::Window &wnd)
    {
        Button::attach(wnd);
        pw->callback(reinterpret_cast<Fl_Callback *>(do_it), pSpinner);
    }
};

class Spinner
{
    static const int radius_step = 5;
    SpButton btn_up, btn_down;
    Graph_lib::Out_box radiusBox;
    Circle *pCirc = nullptr;
    Graph_lib::Window *pW = nullptr;
    int curVal = 5;
    void changeRadius(int radius_step, Graph_lib::Window &win)
    {
        if (curVal + radius_step < 0 ||
            curVal + radius_step > 180)
            return;
        curVal += radius_step;
        radiusBox.put(curVal);
        if (pCirc != nullptr)
        {
            pCirc->set_radius(curVal);
            pCirc->move(-radius_step, -radius_step);
            pW->redraw();
        }
    }
    static void cb_up(Address wnd, Address pSpin)
    {
        Graph_lib::Window &win = reference_to<Graph_lib::Window>(wnd);
        reference_to<Spinner>(pSpin).changeRadius(radius_step, win);
    }
    static void cb_down(Address wnd, Address pSpin)
    {
        Graph_lib::Window &win = reference_to<Graph_lib::Window>(wnd);
        reference_to<Spinner>(pSpin).changeRadius(-radius_step, win);
    }

public:
    Spinner(Point loc, int startVal)
        : btn_up(loc, 20, 20, "/\\", Spinner::cb_up, this),
          btn_down(Point(loc.x + 60, loc.y), 20, 20, "\\/",
                   Spinner::cb_down, this),
          radiusBox(Point(loc.x + 25, loc.y), 30, 20, ""),
          curVal(startVal)
    {
    }
    void attach(Graph_lib::Window &wnd)
    {
        pW = &wnd;
        wnd.attach(btn_up);
        wnd.attach(btn_down);
        wnd.attach(radiusBox);
        radiusBox.put(curVal);
        pCirc = new Circle(Point(wnd.x_max() / 2, wnd.y_max() / 2), curVal);
        wnd.attach(*pCirc);
    }
};

int main()
{
    Graph_lib::Window wnd(Point(100, 100), 600, 400,
                          "Radoslaw Dabkowski (325683 % 8 == 3)");
    Spinner spin(Point((wnd.x_max() - 90) / 2, wnd.y_max() - 20), 100);
    Button btn(Point((wnd.x_max() - 80), wnd.y_max() - 20), 80,
               20, "Close", cb_close);
    wnd.attach(btn);
    spin.attach(wnd);

    return gui_main();
}
