#include "figure.h"
#include <Graph.h>
#include <Simple_window.h>
using namespace std;

typedef pair<FPoint, FPoint> TrPara;
struct RectRes
{
    int x, y, w, h;
    friend ostream& operator<<(ostream& os, const RectRes& rr)
    {
        return os << '{' << rr.x << ',' << rr.y << ',' << rr.w << ',' << rr.h << '}';
    }
};
bool rects_equal(const Graph_lib::Rectangle& glRect, const RectRes& res)
{
    return glRect.point(0).x == res.x && glRect.point(0).y == res.y && glRect.width() == res.w && glRect.height() == res.h;
}
ostream& operator<<(ostream& os, const Graph_lib::Rectangle& rc)
{
    return os << '{' << rc.point(0).x << ',' << rc.point(0).y << ',' << rc.width() << ',' << rc.height() << '}';
}

void RectTrafoTest()
{
    vector<string> rectTest = { "Rect((10, 10)(20, 20))", "Rect((-10, 10)(15, 25))" };
    vector<TrPara> trPara = { {{1.0f, 1.0f}, {0.0f, 0.0f}}, {{1.0f, -1.0f}, {0.0f, 0.0f}}, {{1.0f, -1.0f}, {0.0f, 0.0f}}, {{3.0f, 2.0f}, {0.0f, 0.0f}},
                              {{1.0f, 1.0f}, {10.0f, 10.0f}}, {{1.0f, 1.0f}, {20.0f, -5.0f}}, {{1.0f, 1.0f}, {-10.0f, 10.0f}}, {{2.0f, 4.0f}, {-200.f, -100.f}}
                            };
    vector<RectRes> rcRes = { {10, 10, 10, 10}, {10, -20, 10, 10}, {10,-20,10,10}, {30,20,30,20},
                                {20,20,10,10}, {30,5,10,10}, {0,20,10,10}, {-180,-60,20,40},
                              {-10, 10, 25, 15}, {-10, -25, 25, 15}, {-10,-25,25,15}, {-30,20,75,30},
                                {0,20,25,15}, {10,5,25,15}, {-20,20,25,15}, {-220,-60,50,60}
                            };
    int resIdx = 0;
    for (const auto& str : rectTest)
    {
        stringstream ss(str);
        try
        {
            figure *fig;
            if ((fig = get_figure(ss)) != nullptr)
            {
                if (typeid(*fig) == typeid(Rect))
                {
                    for (const auto& tPara : trPara)
                    {
                        Graph_lib::Shape* pSh = fig->get_shape(tPara.first, tPara.second);
                        if (typeid(*pSh) == typeid(Graph_lib::Rectangle))
                        {
                            Graph_lib::Rectangle* psRect = (Graph_lib::Rectangle*) pSh;
                            if (!rects_equal(*psRect, rcRes[resIdx]))
                            {
                                cout << "Error in Graph_lib::Rectangle coordinates.\n";
                                cout << *psRect << endl;
                                cout << rcRes[resIdx] << endl;

                            }
                        }
                        else
                            cout << "Unexpected shape type.\n";
                        ++resIdx;
                    }
                }
                else
                    cout << "Unexpected figure type.\n";
            }
            else
                cout << "Unexpected nullptr result.\n";
        }
        catch (...)
        {
            cout << "figure: unexpected exception in >>\n";
        }
    }
}

int main()
{
    // testy transformacji dla Rect
    RectTrafoTest();

    // tutaj jest miejsce na umieszczenie swoich testów

    // sprawdzmy, czy czyta sie linia
    string oneLine = "Line((90,30)(85,31)(82,33))";
    stringstream ss(oneLine);
    try
    {
        figure *fig = get_figure(ss);
        {
            cout << *fig << endl;
            cout << fig->bbox() << endl;
        }
    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;
    }
    catch (...)
    {
        cout << "figure: unexpected exception in >>\n";
    }

    // obrazek finalowy
    ifstream ifs("mapa_korona.txt");
    if (!ifs.good()) // dobrze sprawdziæ przed czytaniem
    {
        cout << "Cannot find input file.\n";
        return 1;
    }
    vector<figure*> figs;
    try
    {
        figure *fig;
        while ((fig = get_figure(ifs)) != nullptr)
        {
            cout << *fig << endl;
            figs.push_back(fig);
        }
    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;
    }
    catch (...)
    {
        cout << "figure: unexpected exception in >>\n";
    }
    ifs.close();

    // przygotowanie do wyswietlenia
    pair<FPoint, FPoint> corona_box = map_bbox(figs);
    pair<FPoint, FPoint> win_box = { {20.0f, 20.0f}, {500.f, 380.f} };

    for(unsigned int i = 0; i < figs.size(); ++i)
    {
        cout << i << ": " << figs[i] -> bbox() << endl;
    }

    pair<FPoint, FPoint> trafo = get_transformation(corona_box, win_box);

    cout << trafo << endl;

    Simple_window wnd(Point(100, 100), 600, 400, window_title());
    for (auto pf : figs)
        wnd.attach(*(pf->get_shape(trafo.first, trafo.second)));
    wnd.wait_for_button();

    return 0;
}
