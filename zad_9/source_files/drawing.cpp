#include "drawing.h"

void myWindow::refreshMap()
{
    for (auto &f : figures)
    {
        // detach the old shape
        detach(*f.second);
        delete f.second;
        // create a new shape
        f.first->transform(rotationSpeed, center);
        f.second = f.first->get_shape(scale, transformation);
        // attach the new shape
        attach(*f.second);
    }
    redraw();
}

void myWindow::timer_callback(Address addr)
{
    myWindow *pWnd = static_cast<myWindow *>(addr);
    pWnd->refreshMap();
    if (pWnd->animationRunning)
        Fl::repeat_timeout(0.25, timer_callback, pWnd);
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

void myWindow::attach_fig(figure *f)
{
    auto shape = f->get_shape(scale, transformation);
    figures.push_back(std::make_pair(f, shape));
    attach(*shape);
}
