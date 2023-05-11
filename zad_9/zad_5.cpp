#include "figure.h"
#include "drawing.h"
#include "FPoint.h"

int main()
{
    // obrazek finalowy
    ifstream ifs("house_.txt");
    if (!ifs.good()) // dobrze sprawdziæ przed czytaniem
    {
        cout << "Cannot find input file.\n";
        return 1;
    }
    vector<figure *> figs;
    try
    {
        figure *fig;
        while ((fig = get_figure(ifs)) != nullptr)
        {
            cout << *fig << endl;
            figs.push_back(fig);
        }
    }
    catch (exception &ex)
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
    pair<FPoint, FPoint> win_box = {{20.0f, 20.0f}, {500.f, 380.f}};

    for (unsigned int i = 0; i < figs.size(); ++i)
    {
        cout << i << ": " << figs[i]->bbox() << endl;
    }

    pair<FPoint, FPoint> trafo = get_transformation(corona_box, win_box);

    cout << trafo << endl;

    myWindow wnd(FPoint(100, 100), 600, 400, window_title(), trafo.first, trafo.second);
    for (auto pf : figs)
    {
        wnd.attach_fig(pf);
    }
    Graph_lib::gui_main();
    return 0;
}
