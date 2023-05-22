#pragma once
#include "Window.h"
#include "Graph.h"
#include "GUI.h"

struct colorSpec
{
  std::string label;
  Graph_lib::Color color;
};

class MenuWindow;
class MenuHeader;
class MenuItem;

struct actionDescriptor 
{ 
    enum Action { NoAction, Menu_toggle, Menu_select }; 
    MenuWindow      *pParent = nullptr; 
    MenuHeader      *pMenu = nullptr; 
    Action           menu_action = NoAction; 
    Graph_lib::Color selected_color = Graph_lib::Color::invisible; 
};

class MenuItem : public Graph_lib::Button
{
public:
    MenuItem(Graph_lib::Point loc, int w, int h, const std::string& label) :
        Graph_lib::Button(loc, w, h, label, nullptr)
    {}
    void attach(MenuWindow* pWnd, MenuHeader *pMenu, Graph_lib::Color color, Graph_lib::Callback cb_setColor);
private:
    actionDescriptor buttonAction;
};

class MenuHeader : public Graph_lib::Button {
  std::vector<Graph_lib::Button*> btns;
  actionDescriptor action;
  MenuWindow* pWnd = nullptr;
  bool expanded = false;
  actionDescriptor mAction;

  static void cb_openClose(Graph_lib::Address, Graph_lib::Address pDsc);
  static void cb_setColor(Graph_lib::Address, Graph_lib::Address pAD);
  void hideMenu();
  void showMenu();
public:
  MenuHeader(Graph_lib::Point xy, int w, int h, const std::string& label) :
    Graph_lib::Button(xy, w, h, label, cb_openClose),
    action()
    {}
  ~MenuHeader();
  void openClose();
  void attach(MenuWindow* pWnd, const std::vector<colorSpec>& btns);
};

class MenuWindow : public Graph_lib::Window {
  Graph_lib::Rectangle rect;
  Graph_lib::Button close_btn;
  MenuHeader menu_fill;
  MenuHeader menu_line;

  static void cb_close(Graph_lib::Address, Graph_lib::Address pw){
    Graph_lib::reference_to<MenuWindow>(pw).hide();
  }
public:
    MenuWindow(Graph_lib::Point loc, int w, int h, const std::string& label);
    const static std::vector<colorSpec> fill_colors;
    void setColor(Graph_lib::Color color);
    void menuAction(actionDescriptor* action);
};


/*

6
JTP2 lato 2022/2023 Ćwiczenia 12 1/19 
Obsługa menu w 25 kroków 
Inspiracją do kolejnego zadania było rozwiązanie zadania, które 
dostałem w jednej z poprzednich edycji: 
 
Niewątpliwie wodotrysków dodanych do rysowania prostokątów jest tutaj 
sporo (dawały dodatkowe dwa punkty), ale zostały wykonane brutalnymi 
metodami. Można zrobić to zgrabniej, oszczędzając i miejsce w oknie, i 
ilość pisanego kodu. 
JTP2 lato 2022/2023 Ćwiczenia 12 2/19 
Problem mnóstwa przycisków 
Mnogość przycisków zajęła nie tylko cenne miejsce w obszarze 
okna, ale też dała mnóstwo, bardzo do siebie podobnych, 
fragmentów kodu. O ile na tym etapie jest zrozumiałe, że każdy 
przycisk miał swoją osobną funkcję callback (cb_fillred, 
cb_fillyellow, itd.), to powielenie podobnego zestawu 
bezparametrowych metod w myWindow było niewątpliwie wynikiem 
inercji (i sprawnego działania schowka). 
W tym zadaniu trzeba utworzyć specyficzne „menu” wykorzystujące 
przyciski z biblioteki Graph_lib. Żeby skoncentrować się na 
podstawowej części zadania, w obszarze rysowania umieścimy tylko 
jeden kształt (prostokąt). Temu prostokątowi będziemy zmieniać 
kolor wypełnienia i konturu korzystając z dwóch menu do zmiany 
koloru. 
 
JTP2 lato 2022/2023 Ćwiczenia 12 3/19 
Funkcjonalność menu 
Funkcjonalność menu, którą chcemy uzyskać można 
podsumować w trzech punktach: 
1. Kliknięcie przycisku menu powinno spowodować 
wyświetlenie pod nim szeregu przycisków ustawiania 
koloru (rozwinięcie menu).  
2. Kliknięcie przycisku menu przy rozwiniętej liście 
przycisków powinno zwinąć menu. 
3. Kliknięcie przycisku koloru powinno zwinąć menu i 
ustawić stosowny kolor kształtu. 
Tak wygląda to z punktu widzenia użytkownika; faktycznie, 
kliknięcia będą po prostu generować komunikaty 
przesyłane do właściwego adresata, po to, żeby na końcu 
obiekt okna zainicjował odpowiednią akcję.  
JTP2 lato 2022/2023 Ćwiczenia 12 4/19 
Wymagania niefunkcjonalne 
W trakcie tworzenia programu chciałbym: 
1. Użyć możliwie niewielu funkcji callback. 
2. Posługiwać się dobrze zdefiniowanymi strukturami 
(nazwy składowych) i wyliczeniami (coś mówiące 
identyfikatory). 
3. Implementowanie metod i funkcji tylko w pliku 
mywindow.cpp. 
4. Kompilować tworzony kod możliwie jak najczęściej; 
uruchamiać go o ile tylko wynik kompilacji na to 
pozwala. 
5. Po uzyskaniu pełnej funkcjonalności sprawdzić ją 
uważnie a następnie przejrzeć kod i uporządkować 
go. 
JTP2 lato 2022/2023 Ćwiczenia 12 5/19 
Efekt końcowy 
 
JTP2 lato 2022/2023 Ćwiczenia 12 6/19 
Pierwsze kroki 
32 kroki prowadzące do końcowego programu wyglądają 
następująco: 
1. Utworzenie nowego projektu (zad_8) na podstawie 
„standardowego” szablonu z biblioteką Graph_lib. 
2. Zmiana nazwy pliku testu na zad_8.cpp. 
3. Utworzenie klasy MenuWindow, dziedziczącej publicznie po 
Graph_lib::Window, rozdzielonej na plik menuwindow.h 
(definicje klas, struktur i wyliczeń) oraz menuwindow.cpp 
(implementacje metod). 
4. Dodanie przycisku Close do zamykania okna i sprawdzenie 
jego działania. 
5.  Dodanie kształtu (Graph_lib::Rectangle) jako standardowej 
składowej MenuWindow; inicjowanie w liście inicjacyjnej 
konstruktora MenuWindow.  
JTP2 lato 2022/2023 Ćwiczenia 12 7/19 
6. Zdefiniowanie klasy MenuHeader dziedziczącej publicznie po 
Graph_lib::Button.  
7. Zdefiniowanie konstruktora klasy MenuHeader: 
 MenuHeader(Graph_lib::Point loc, int w, int h, 
  const std::string& label); 
Jak widać, w konstruktorze nie ma wskazania na funkcję 
Callback – w tworzonym w konstruktorze przycisku trzeba 
ustawić ją na nullptr. 
8. Dodanie do MenuWindow składowej btn_fill (typu 
MenuHeader) i zainicjowanie jej w konstruktorze. Z pewnych 
względów umieśćmy górny lewy róg we współrzędnych 
(50, 50). 
9. Sprawdzenie, że przycisk Fill jest widoczny w oknie (trzeba 
pamiętać o wywołaniu attach() – jak dla zwykłego przycisku 
– w konstruktorze MenuWindow). 
JTP2 lato 2022/2023 Ćwiczenia 12 8/19 
10. Ponieważ informację o etykietach przycisków i kolorach chcę 
przekazywać do funkcji MenuHeader::attach(), to najpierw 
należy zdefiniować strukturę colorSpec: 
struct colorSpec 
{ 
 std::string      label; 
 Graph_lib::Color color; 
}; 
11. Teraz mogę zająć się funkcją attach() wmMenuHeader: 
 void attach(MenuWindow* pWnd, 
  const std::vector<colorSpec>& btns); 
12.  Do wywołania będą mi potrzebne kolory; definiuję stosowną 
statyczną i prywatną składową w klasie MenuWindow: 
 static std::vector<colorSpec> fill_colors; 

*/