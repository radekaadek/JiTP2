#pragma once
#include "Simple_window.h"
#include "figure.h"

class myWindow : public Graph_lib::Window
{
public:
    myWindow(Graph_lib::Point loc, int w, int h, const std::string &title, const std::vector<figure *> figs) :
            Graph_lib::Window(loc, w, h, title),
            animate(Graph_lib::Point(0, 0), 70, 20, "Start", cb_toggleAnimation),
            close(Graph_lib::Point(0, 20), 70, 20, "Close", cb_close)
            {
                // przygotowanie do wyswietlenia
                std::pair<FPoint, FPoint> fig_box = map_bbox(figs);

                for (unsigned int i = 0; i < figs.size(); ++i)
                {
                    cout << i << ": " << figs[i]->bbox() << endl;
                }

                pair<FPoint, FPoint> trafo = get_transformation(fig_box, win_box);
                scale = trafo.first;
                transformation = trafo.second;
                attach_figs(figs);
                attach(animate);
                attach(close);
                // nie wiem gdzie jest srodek :(
                center = {(win_box.first.x + win_box.second.x) / 2,
                          (win_box.first.y + win_box.second.y) / 2};
            };
    ~myWindow()
    {
        for (auto &f : figures)
        {
            detach(*f.second);
            delete f.second;
            delete f.first;
        }
    };
    void attach_fig(figure *f);
    void attach_figs(std::vector<figure *> figs);
private:
    Button animate, close;
    FPoint scale = {1, 1}, transformation = {0, 0}, center = {0, 0};
    // const pair<FPoint, FPoint> win_box = {{20.0f, 20.0f}, {500.f, 380.f}};
    const pair<FPoint, FPoint> win_box = {{20.0f, 20.0f}, {500.f, 380.f}};
    std::vector<std::pair<figure *, Graph_lib::Shape *>> figures;
    bool animationRunning = false;
    float rotationAngle = 0.00f;
    static void cb_toggleAnimation(Address, Address pw)
    {
        reference_to<myWindow>(pw).toggleAnimation();
    }
    static void cb_close(Address, Address pw)
    {
        reference_to<myWindow>(pw).hide();
    }
    void toggleAnimation();
    static void timer_callback(Address addr);
    void refreshMap();
    std::vector<figure*> get_figures() const;
    std::pair<FPoint, FPoint> shape_bbox() const;
};
