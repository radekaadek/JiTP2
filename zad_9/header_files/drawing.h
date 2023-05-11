#pragma once
#include "Simple_window.h"
#include "figure.h"


class myWindow : public Graph_lib::Window {
public:
    myWindow(Graph_lib::Point loc, int w, int h, const std::string& title, FPoint scale = { 1.0f, 1.0f}, FPoint translation = { 0.0f, 0.0f }) :
        Graph_lib::Window(loc, w, h, title),
        animate(Graph_lib::Point(0, 0), 70, 20, "Start/Stop", cb_toggleAnimation),
        close(Graph_lib::Point(0, 20), 70, 20, "Close", cb_close),
        scale(scale), transformation(translation)
        {
            attach(animate);
            attach(close);
            center = { (float)x_max() / 2.0f, (float)y_max() / 2.0f };
        }
    ~myWindow()
    {
        for (auto& f : figures) {
            detach(*f.second);
            delete f.second;
            delete f.first;
        }
    };
    void attach_fig(figure* f)
    {
        auto shape = f->get_shape(scale, transformation);
        figures.push_back(std::make_pair(f, shape));
        attach(*shape);
    }
private:
    Button animate, close;
    FPoint scale, transformation, center;
    std::pair<FPoint, FPoint> traflo;
    std::vector<std::pair<figure*, Graph_lib::Shape*>> figures;
    bool animationRunning = false;
    float rotationAngle = 0.0f;
    static void cb_toggleAnimation(Address, Address pw)
    {
        reference_to<myWindow>(pw).toggleAnimation();
    }
    static void cb_close(Address, Address pw)
    {
        reference_to<myWindow>(pw).hide();
    }
    void toggleAnimation()
    {
        animationRunning = !animationRunning;
        if (animationRunning) {
            // change the button label
            animate.label = "Stop";
            Fl::add_timeout(0.25, timer_callback, this);
        }
        else {
            Fl::remove_timeout(timer_callback, this);
            animate.label = "Start";
        }
    }
    static void timer_callback(Address addr)
    {
        myWindow* pWnd = static_cast<myWindow*>(addr);
        pWnd->rotationAngle += 0.01f;
        pWnd->refreshMap();
        if (pWnd->animationRunning)
            Fl::repeat_timeout(0.25, timer_callback, pWnd);
    }
    void refreshMap()
    {
        for (auto& f : figures) {
            f.first->rotate(rotationAngle, center);
            // detach the old shape
            detach(*f.second);
            delete f.second;
            // create a new shape
            f.second = f.first->get_shape(scale, transformation);
            // attach the new shape
            attach(*f.second);
        }
        redraw();
    }

};
