#include "drawing.h"

void myWindow::refreshMap()
{
    for (auto &f : figures)
    {
        // detach the old shape
        detach(*f.second);
        delete f.second;
        // create a new shape
        f.second = f.first->transform(rotationAngle, center, scale, transformation);
        // attach the new shape
        attach(*f.second);
    }
    redraw();
}

void myWindow::timer_callback(Address addr)
{
    myWindow *pWnd = static_cast<myWindow *>(addr);
    pWnd->refreshMap();
    pWnd->rotationAngle += 0.05f;
    if (pWnd->animationRunning)
        Fl::repeat_timeout(0.10, timer_callback, pWnd);
}

void myWindow::toggleAnimation()
{
    animationRunning = !animationRunning;
    if (animationRunning)
    {
        // change the button label
        animate.label = "Stop";
        Fl::add_timeout(0.25, timer_callback, this);
    }
    else
    {
        Fl::remove_timeout(timer_callback, this);
        animate.label = "Start";
    }
}

std::vector<figure*> myWindow::get_figures() const
{
    std::vector<figure *> figs;
    for (auto &f : figures)
    {
        figs.push_back(f.first);
    }
    return figs;
}

void myWindow::attach_fig(figure *f)
{
    auto shape = f->transform(rotationAngle, center, scale, transformation);
    figures.push_back(std::make_pair(f, shape));
    auto figures = get_figures();
    corona_box = map_bbox(figures);
    attach(*shape);
}
