#pragma once
#include "Simple_window.h"
#include "figure.h"

// define adress as a pointer to a function
typedef void* Address;

class myWindow : public Graph_lib::Window {
public:
    myWindow(Graph_lib::Point loc, int w, int h, const std::string& title) :
        Graph_lib::Window(loc, w, h, title),
        animate(Graph_lib::Point(0, 0), 70, 20, "Start/Stop", cb_toggleAnimation),
        close(Graph_lib::Point(0, 20), 70, 20, "Close", cb_close)
        {
            attach(animate);
            attach(close);
        }
    ~myWindow()
    {
        for (auto& f : figures)
            delete f;
    };
    void add_figure(figure* f) { figures.push_back(f); }
    void stopAnimation() { animationRunning = false; }
private:
    std::vector<figure*> figures;
    bool animationRunning = false;
    float rotationAngle = 0.0f;
    Button animate, close;
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
        if (animationRunning)
            Fl::add_timeout(0.25, timer_callback, this);
        else
            Fl::remove_timeout(timer_callback, this);
    }
    static void timer_callback(Address addr)
    {
        myWindow* pWnd = static_cast<myWindow*>(addr);
        pWnd->rotationAngle += 0.05f;
        pWnd->refreshMap();
        if (pWnd->animationRunning)
            Fl::repeat_timeout(0.25, timer_callback, pWnd);
    }
    void refreshMap()
    {
        redraw();
    }

};

// void myWindow::timer_callback(Address addr)
// { 
//  myWindow *pWnd = static_cast<myWindow*>(addr); 
//  pWnd->rotationAngle += 0.05f; 
//  pWnd->refreshMap(); 
//  if (pWnd->animationRunning) 
//     Fl::repeat_timeout(0.25, timer_callback, pWnd);
// }

// void myWindow::toggleAnimation()
// {
//     animationRunning = !animationRunning;
//     if (animationRunning)
//         Fl::add_timeout(0.25, timer_callback, this);
//     else
//         Fl::remove_timeout(timer_callback, this);
// }

/*
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
 
Interfejs programu 
 
Uwaga: przyjmijmy, że środek obrotu jest w środku pola prezentacji; pamiętamy 
go oczywiście w obiekcie klasy myWindow. 
Zmieniana etykieta 
przycisku 
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
JTP2 lato 2022/2023 Ćwiczenia 10 11/14 
Wszystko łamaną 
Oczywiście, prostokąt zamienił się w łamaną! 
Na pierwszy rzut oka może się wydawać, że podejście do 
prostokątów i kół będzie wymagało sporo dodatkowego 
zachodu: wyciągamy za pomocą get_shape() stosowny 
kształt i kombinujemy co dalej. 
Ja sugerowałbym rozwiązanie śmiałe i oszczędzające sporo 
pracy. Klasę figure rozbudowałbym o wirtualną metodę 
get_points, dającą wektor punktów łamanej reprezentujący 
daną figurę. Domyślna implementacja zwraca po prostu kopię 
fdef (czyli jest dobra dla Line), natomiast dla Rect trzeba 
wyliczyć stosowną piątkę punktów (używamy Open_polyline). 
Z Circ będzie jeszcze więcej zabawy ☺ 
Ale za to wyświetlanie będzie proste!  
(Jedna funkcja transformująca punkty, bo tylko punkty nam zostały). 
Zatrzymanie animacji 
Pozostaje ustalić, w jaki sposób zatrzymać animację. 
Jeśli scena kręci się w oknie, to informację o naciśnięciu Stop 
(to ten sam przycisk, co Start, tylko ma inną etykietę) 
dostaniemy, kiedy timer_callback czeka, aż upłynie czas do 
wyświetlenia kolejnej ramki animacji. 
Podpowiedź jest zawarta w kodzie timer_callback: do 
zatrzymania animacji wystarczy nie wołać ponownie powtórki; 
sprawę powinno zatem załatwić ustawienie 
animationRunning na false.  
Zostanie jeszcze delikatna sprawa przycisku Close – czy 
można bez szkody zamknąć okno w trakcie animacji? 
 
Reorganizacja kodu 
Projekt jest stosunkowo niewielki, ale już teraz pojawia się w nim 
bardzo niedobry objaw: wszystko zależy od wszystkiego. 
Żeby skorzystać z FPoint lub figure, musimy włączyć do projektu 
graph_lib, bo obie klasy zależą od graph_lib. 
Damy temu odpór reorganizując kod: definicja FPoint pójdzie do 
osobnego pliku, a wiązanie z graph_lib usuniemy, usuwając 
operator konwersji typu (zdefiniujemy go „przy oknie”). 
Definicje figur (i funkcji pomocniczych) zostaną w plikach figure, a 
powiązanie z graph_lib wyrugujemy pozbywając się metody 
get_shape (zastąpi ją get_points, która zwraca wektor FPoint). 
Macierze powinny zależeć co najwyżej od FPoint.  
W ten sposób graph_lib będzie potrzebne tylko i wyłącznie w 
plikach drawing (implementujących myWindow). 
Oddawanie 
Rozwiązanie zadania składa się z 8 plików: 
fpoint.h – zawiera definicję FPoint i funkcji operujących 
fpoint.cpp – zawiera implementacje FPoint 
figure.h  – zawiera definicje klas figur i deklaracje funkcji 
pomocniczych 
figure.cpp  – implementacje klas figur i f. pomocniczych 
drawing.h – definicje klas obsługi interfejsu 
drawing.cpp – implementacje klas obsługi interfejsu 
matrix.h – implementacja szablonu klasy matrix 
anim_test.cpp – funkcja main. 
Rozwiązanie (spakowane w archiwum zip) proszę złożyć 
w Moodle do: 
14 maja 2023 23:59 
 
*/