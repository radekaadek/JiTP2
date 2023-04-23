// Radoslaw Dabkowski (325683)

#include "Simple_window.h"
#include <random>

ostream& operator<<(ostream& os, const Point& pt)
{
    return os << '(' << pt.x << ',' << pt.y << ')';
}

ostream& operator<<(ostream& os, const Graph_lib::Rectangle& rect)
{
    Point tl = rect.point(0);
    Point br = Point(rect.point(0).x + rect.width(), rect.point(0).y + rect.height());
    return os << "Rect (" << tl << br << ")";
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
    
    std::pair<Point, Point> intersect(Point pt1, Point pt2) const
    {
        int x1 = max(min(pt1.x, pt2.x), top_left.x);
        int y1 = max(min(pt1.y, pt2.y), top_left.y);
        int x2 = min(max(pt1.x, pt2.x), bottom_right.x);
        int y2 = min(max(pt1.y, pt2.y), bottom_right.y);
        return {Point(x1, y1), Point(x2, y2)};
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

    Graph_lib::Rectangle *getRect(Point pt1, Point pt2)
    {
        Point top_left = Point(min(pt1.x, pt2.x), min(pt1.y, pt2.y));
        Point bottom_right = Point(max(pt1.x, pt2.x), max(pt1.y, pt2.y));
        return new Graph_lib::Rectangle(top_left, bottom_right);
    }

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
        pRect = getRect(start, pt);
        pRect->set_color(random_color()); // :)
        attach(*pRect);
        redraw();
        return;
    }

    void stopDrawing(Point stop)
    {
        drawingNow = false;

        // Draw a new rectangle confined to the canvas
        auto coords = canvas.intersect(start, stop);
        auto rect = getRect(coords.first, coords.second);
        rect->set_color(random_color()); // :)
        shapes.push_back(rect);
        attach(*rect);

        // Delete the temporary rectangle
        detach(*pRect);
        delete pRect;
        pRect = nullptr;

        redraw();
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
        for (auto s : shapes) {
            // check if s is a rectangle
            if (auto rect = dynamic_cast<Graph_lib::Rectangle *>(s)) {
                ofs << *rect << '\n';
            }
            else {
                cerr << "Unknown shape\n";
            }
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
    ~rectWindow()
    {
        for (auto s : shapes)
            delete s;
        if (pRect)
            delete pRect;
    }
};


int main()
{
    rectWindow wnd(Point(100, 100), 600, 400,
        "Radoslaw Dabkowski (325683)");
    gui_main();
    return 0;
}
