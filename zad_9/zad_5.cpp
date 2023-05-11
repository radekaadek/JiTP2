#include "Matrix.h"
#include "figure.h"
#include "drawing.h"
#include "FPoint.h"

/*

7
JTP2 lato 2022/2023 Ćwiczenia 10 1/14 
Animacja 
Zadanie kolejne polega na zaimplementowaniu animacji odczytanej 
z pliku mapy. Zupełnie wystarczające będzie obracanie mapy 
względem środka obszaru prezentacji. 
W tym zadaniu będzie istotne zarządzanie pamięcią: wycieki 
pamięci przy animacji mogą mieć fatalne skutki.  
Minimum wystarczającym do zaliczenia tego ćwiczenia (a nawet 
uzyskania najwyższej liczby punktów) jest właściwa obsługa 
transformacji dla łamanej (Line). 
Za każdą dodatkową figurę (maksymalnie 2), która będzie zgrabnie 
animowana w oknie będzie można dostać dodatkowy punkt 
(ostatnie małe zadanie). 
Ważne jest dopracowanie szczegółów: odpowiednia obsługa 
zdarzeń timera i zadbanie o aktualizację elementów interfejsu 
(przycisk Start/Stop). 
JTP2 lato 2022/2023 Ćwiczenia 10 2/14 
Okno animacji 
  
Uwaga: przyjmijmy, że środek obrotu jest w środku pola prezentacji; u mnie jest to 
punkt (250,200). 
Zmieniana etykieta 
przycisku 
JTP2 lato 2022/2023 Ćwiczenia 10 3/14 
Okno animacji 
  
JTP2 lato 2022/2023 Ćwiczenia 10 4/14 
Elementy animacji 
Do zrealizowania animacji trzeba zaimplementować macierze 
transformacji geometrycznych. 
Szczegóły są dokładnie omówione w wykładzie #8. 
Infrastrukturę, która pozwoli nam na zrealizowanie animacji, 
omówimy w trakcie pierwszej części ćwiczeń.  
Żeby sprawy nie komplikować zanadto przyjmiemy, że 
wszystkie obiekty sceny będą animowane tak samo, tzn. będą 
obracać się wokół ustalonego punktu (finalnie) – wyznaczenie 
macierzy transformacji będzie stosunkowo proste i pozwoli 
skoncentrować się na innych elementach związanych z 
animacją. Przyjmujemy, że scenę czytamy z pliku (wracają 
figury!) i że użytkownik może uruchamiać i zatrzymywać 
animację w dowolnym momencie. 
JTP2 lato 2022/2023 Ćwiczenia 10 5/14 
Jak nie animować 
To ostatnie założenie wyklucza podejście, które początkującym 
animatorom narzuca się z ogromną nachalnością: while (nie_koniec_animacji) 
{ 
 Zmodyfikuj_parametry_animacji 
 Wyznacz_macierz_transformacji 
 Przelicz_współrzędne_figur 
 Odepnij_poprzedni_zestaw_kształtów 
 Przypnij_nowy_zestaw_kształtów 
} 
Podstawowy problem tego podejścia, to brak przekazania 
sterowania do biblioteki graficznej (czyli nie dość, że musimy 
określić „koniec animacji” wcześniej, to w trakcie animacji 
aplikacja nie będzie odpowiadać na akcje użytkownika).  
JTP2 lato 2022/2023 Ćwiczenia 10 6/14 
Zdarzenia związane z czasem 
Warto wspomnieć o jeszcze jednym mankamencie 
poprzedniego podejścia: szybkość animacji zależy od 
wydajności komputera, na którym działa program. 
Potrzebna nam będzie informacja o upływie czasu, najlepiej 
dostarczona z biblioteki obsługującej okna – chodzi o to, żeby 
pomiędzy momentami (krótkiej) aktywności naszego kodu 
związanej z animacją interfejs użytkownika odpowiadał na 
akcje użytkownika. 
Podstawową funkcją w fltk, która pozwala na uruchomienie 
wskazanej funkcji po upływie zadanego czasu, jest: 
void Fl::add_timeout(double t, Fl_Timeout_Handler cb, 
void * argp = 0)  
 
JTP2 lato 2022/2023 Ćwiczenia 10 7/14 
Zdarzenia związane z czasem 
Tajemnicze Fl_Timeout_Handler jest zdefiniowane tak: 
typedef void(* Fl_Timeout_Handler) (void *data); 
Ewidentnie funkcja zwrotna z jednym parametrem (i ani chybi 
argument data dla funkcji zwrotnej zadajemy w argp). 
add_timeout wystarczyłby na dobrą sprawę do animacji, ale 
mamy jeszcze drugą funkcję: 
void Fl::repeat_timeout(double t, Fl_Timeout_Handler cb, 
void * argp = 0) 
Różnica między add i repeat jest subtelna: add odlicza czas 
od momentu wywołania, natomiast repeat od zakończenia 
poprzedniego odliczania czasu. 
 
JTP2 lato 2022/2023 Ćwiczenia 10 8/14 
Interfejs programu 
 
Uwaga: przyjmijmy, że środek obrotu jest w środku pola prezentacji; pamiętamy 
go oczywiście w obiekcie klasy myWindow. 
Zmieniana etykieta 
przycisku 
JTP2 lato 2022/2023 Ćwiczenia 10 9/14 
Główny animator 
Głównym animatorem jest funkcja callback, która zadaje, za 
jaki czas ma być „wzbudzona” ponownie: 
void myWindow::timer_callback(Address addr) 
{ 
 myWindow *pWnd = static_cast<myWindow*>(addr); 
 pWnd->rotationAngle += 0.05f; 
 pWnd->refreshMap(); 
 if (pWnd->animationRunning) 
  Fl::repeat_timeout(0.25, timer_callback, pWnd); 
} 
Bez pierwszego uruchomienia tej funkcji się nie obejdzie, ale 
sprawa jest dość oczywista: w reakcji na naciśnięcie Start 
wołamy add_timeout (to oczywiście niejedyna rzecz, jaką 
trzeba zrobić po naciśnięciu Start). 
„Parametry” animacji 
Żądanie ponownego uruchomienia funkcji po określonym czasie. 
JTP2 lato 2022/2023 Ćwiczenia 10 10/14 
Problemy z kształtami 
 
Zaznaczony na obrazku przycisk ma zmienioną etykietę, ale to 
najmniejszy problem. Czy widzicie o wiele poważniejszy? 

*/

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

int main() {
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
    ifstream ifs("house_.txt");
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

    myWindow wnd(FPoint(100, 100), 600, 400, window_title(), trafo.first, trafo.second);
    for (auto pf : figs) {
        auto shape = pf->get_shape(trafo.first, trafo.second);
        wnd.attachFig(pf, shape);
    }
    Graph_lib::gui_main();
    return 0;
}
