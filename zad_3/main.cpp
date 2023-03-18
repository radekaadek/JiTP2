#include <Graph.h>
#include <Simple_window.h>

// Compile with:
// g++ -o example.out main.cpp $(find graph_lib/ -name "*.cpp") -I graph_lib/ -lfltk -lfltk_images -lX11 -lXext -O4 && ./example.out

int main()
{
    Simple_window wnd(Point(100, 100), 600, 400, "Radoslaw Dabkowski (325683)");
    Graph_lib::Polygon poly;
    poly.add(Point(100, 100));
    poly.add(Point(100, 200));
    poly.add(Point(200, 200));
    poly.set_color(Color::red);
    wnd.attach(poly);
    wnd.wait_for_button();
    poly.add(Point(200, 100));
    wnd.wait_for_button();
    return 0;
}
