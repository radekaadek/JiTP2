// Radoslaw Dabkowski (325683)

#include "Simple_window.h"
#include <random>

ostream& operator<<(ostream& os, const Point& pt)
{
    return os << '(' << pt.x << ',' << pt.y << ')';
}

ostream& operator<<(ostream& os, const Graph_lib::Rectangle& rect)
{
    return os << "Rect " << rect.point(0) << ' ' << rect.point(1);
}

class DrawingArea {
    Point top_left, bottom_right;
public:
    DrawingArea(Point tl, Point br) : top_left(tl), bottom_right(br) {}
    bool point_inside(Point pt) const
    {
        bool x_inside = pt.x >= top_left.x && pt.x <= bottom_right.x;
        bool y_inside = pt.y >= top_left.y && pt.y <= bottom_right.y;
        return x_inside && y_inside;
    }
};

class rectWindow : public Graph_lib::Window
{
    Button btn_close, btn_save;
    bool drawingNow = false;
    DrawingArea canvas;
    Point start;
    Graph_lib::Rectangle *pRect;
    vector<Shape *> shapes;
    vector<std::pair<Point, Point>> rects;

    Graph_lib::Color::Color_type random_color() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 15);
        return static_cast<Graph_lib::Color::Color_type>(dis(gen));
    }

    void startDrawing(Point pt)
    {
        start = pt;
        drawingNow = true;
    }

    void draw(Point pt)
    {
        if (start.x == pt.x || start.y == pt.y)
            return;
        if (pRect) {
            detach(*pRect);
            delete pRect;
        }
        Point top_left = Point(min(start.x, pt.x), min(start.y, pt.y));
        Point bottom_right = Point(max(start.x, pt.x), max(start.y, pt.y));
        pRect = new Graph_lib::Rectangle(top_left, bottom_right);
        pRect->set_color(random_color()); // :)
        attach(*pRect);
        redraw();
        return;
    }

    void stopDrawing(Point stop)
    {
        if (!canvas.point_inside(stop)) {
            drawingNow = false;
            detach(*pRect);
            delete pRect;
        }
        else {
            drawingNow = false;
            rects.push_back(make_pair(start, stop));
            shapes.push_back(pRect);
        }
        pRect = nullptr;
        redraw();
        return;
    }

    static void cb_close(Address, Address pw)
    {
        reference_to<rectWindow>(pw).hide();
    }

    static void cb_save(Address, Address pw)
    {
        reference_to<rectWindow>(pw).save();
    }

    void save()
    {
        ofstream ofs("rect.txt");
        for (auto& rect : rects) {
            ofs << "Rect " << rect.first << ' ' << rect.second << endl;
        }
    }

    Point event_xy() const
    {
        return Point(Fl::event_x(), Fl::event_y());
    }

    int handle(int event)
    {
        switch (event) {
            case FL_PUSH: // nacisniecie lewego przycisku myszy
                if (!canvas.point_inside(event_xy()))
                    break;
                startDrawing(event_xy());
                break;
            case FL_DRAG: // "wleczenie" myszy
                if (drawingNow)
                    draw(event_xy());
                break;
            case FL_RELEASE: // zwolnienie lewego przycisku myszy
                if (drawingNow)
                    stopDrawing(event_xy());
                break;
        }
        return Fl_Window::handle(event);
    }

public:
    rectWindow(Point loc, int w, int h, string title) :
        Graph_lib::Window(loc, w, h, title),
        btn_close(Point(x_max() - 80, y_max() - 20), 80, 20,
        "Close", rectWindow::cb_close),
        btn_save(Point(x_max() - 80, y_max() - 50), 80, 20,
        "Save", rectWindow::cb_save),
        drawingNow(false),
        canvas(Point(10,10), Point(x_max()-100, y_max()-10)),
        start(0, 0), pRect(nullptr)
    {
        attach(btn_close);
        attach(btn_save);
    }
};


int main()
{
    rectWindow wnd(Point(100, 100), 600, 400,
        "Radoslaw Dabkowski (325683)");
    gui_main();
    return 0;
}
